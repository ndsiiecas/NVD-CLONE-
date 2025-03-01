int CVE_2011_1023_PATCHED_rds_ib_xmit(struct rds_connection *conn, struct rds_message *rm,
		unsigned int hdr_off, unsigned int sg, unsigned int off)
{
	struct rds_ib_connection *ic = conn->c_transport_data;
	struct ib_device *dev = ic->i_cm_id->device;
	struct rds_ib_send_work *send = NULL;
	struct rds_ib_send_work *first;
	struct rds_ib_send_work *prev;
	struct ib_send_wr *failed_wr;
	struct scatterlist *scat;
	u32 pos;
	u32 i;
	u32 work_alloc;
	u32 credit_alloc = 0;
	u32 posted;
	u32 adv_credits = 0;
	int send_flags = 0;
	int bytes_sent = 0;
	int ret;
	int flow_controlled = 0;
	int nr_sig = 0;

	BUG_ON(off % RDS_FRAG_SIZE);
	BUG_ON(hdr_off != 0 && hdr_off != sizeof(struct rds_header));

	/* Do not send cong updates to IB loopback */
	if (conn->c_loopback
	    && rm->m_inc.i_hdr.h_flags & RDS_FLAG_CONG_BITMAP) {
		rds_cong_map_updated(conn->c_fcong, ~(u64) 0);
		scat = &rm->data.op_sg[sg];
		ret = sizeof(struct rds_header) + RDS_CONG_MAP_BYTES;
		ret = min_t(int, ret, scat->length - conn->c_xmit_data_off);
		return ret;
	}

	/* FIXME we may overallocate here */
	if (be32_to_cpu(rm->m_inc.i_hdr.h_len) == 0)
		i = 1;
	else
		i = ceil(be32_to_cpu(rm->m_inc.i_hdr.h_len), RDS_FRAG_SIZE);

	work_alloc = rds_ib_ring_alloc(&ic->i_send_ring, i, &pos);
	if (work_alloc == 0) {
		set_bit(RDS_LL_SEND_FULL, &conn->c_flags);
		rds_ib_stats_inc(s_ib_tx_ring_full);
		ret = -ENOMEM;
		goto out;
	}

	if (ic->i_flowctl) {
		credit_alloc = rds_ib_send_grab_credits(ic, work_alloc, &posted, 0, RDS_MAX_ADV_CREDIT);
		adv_credits += posted;
		if (credit_alloc < work_alloc) {
			rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc - credit_alloc);
			work_alloc = credit_alloc;
			flow_controlled = 1;
		}
		if (work_alloc == 0) {
			set_bit(RDS_LL_SEND_FULL, &conn->c_flags);
			rds_ib_stats_inc(s_ib_tx_throttle);
			ret = -ENOMEM;
			goto out;
		}
	}

	/* map the message the first time we see it */
	if (!ic->i_data_op) {
		if (rm->data.op_nents) {
			rm->data.op_count = ib_dma_map_sg(dev,
							  rm->data.op_sg,
							  rm->data.op_nents,
							  DMA_TO_DEVICE);
			rdsdebug("ic %p mapping rm %p: %d\n", ic, rm, rm->data.op_count);
			if (rm->data.op_count == 0) {
				rds_ib_stats_inc(s_ib_tx_sg_mapping_failure);
				rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
				ret = -ENOMEM; /* XXX ? */
				goto out;
			}
		} else {
			rm->data.op_count = 0;
		}

		rds_message_addref(rm);
		ic->i_data_op = &rm->data;

		/* Finalize the header */
		if (test_bit(RDS_MSG_ACK_REQUIRED, &rm->m_flags))
			rm->m_inc.i_hdr.h_flags |= RDS_FLAG_ACK_REQUIRED;
		if (test_bit(RDS_MSG_RETRANSMITTED, &rm->m_flags))
			rm->m_inc.i_hdr.h_flags |= RDS_FLAG_RETRANSMITTED;

		/* If it has a RDMA op, tell the peer we did it. This is
		 * used by the peer to release use-once RDMA MRs. */
		if (rm->rdma.op_active) {
			struct rds_ext_header_rdma ext_hdr;

			ext_hdr.h_rdma_rkey = cpu_to_be32(rm->rdma.op_rkey);
			rds_message_add_extension(&rm->m_inc.i_hdr,
					RDS_EXTHDR_RDMA, &ext_hdr, sizeof(ext_hdr));
		}
		if (rm->m_rdma_cookie) {
			rds_message_add_rdma_dest_extension(&rm->m_inc.i_hdr,
					rds_rdma_cookie_key(rm->m_rdma_cookie),
					rds_rdma_cookie_offset(rm->m_rdma_cookie));
		}

		/* Note - rds_ib_piggyb_ack clears the ACK_REQUIRED bit, so
		 * we should not do this unless we have a chance of at least
		 * sticking the header into the send ring. Which is why we
		 * should call rds_ib_ring_alloc first. */
		rm->m_inc.i_hdr.h_ack = cpu_to_be64(rds_ib_piggyb_ack(ic));
		rds_message_make_checksum(&rm->m_inc.i_hdr);

		/*
		 * Update adv_credits since we reset the ACK_REQUIRED bit.
		 */
		if (ic->i_flowctl) {
			rds_ib_send_grab_credits(ic, 0, &posted, 1, RDS_MAX_ADV_CREDIT - adv_credits);
			adv_credits += posted;
			BUG_ON(adv_credits > 255);
		}
	}

	/* Sometimes you want to put a fence between an RDMA
	 * READ and the following SEND.
	 * We could either do this all the time
	 * or when requested by the user. Right now, we let
	 * the application choose.
	 */
	if (rm->rdma.op_active && rm->rdma.op_fence)
		send_flags = IB_SEND_FENCE;

	/* Each frag gets a header. Msgs may be 0 bytes */
	send = &ic->i_sends[pos];
	first = send;
	prev = NULL;
	scat = &ic->i_data_op->op_sg[sg];
	i = 0;
	do {
		unsigned int len = 0;

		/* Set up the header */
		send->s_wr.send_flags = send_flags;
		send->s_wr.opcode = IB_WR_SEND;
		send->s_wr.num_sge = 1;
		send->s_wr.next = NULL;
		send->s_queued = jiffies;
		send->s_op = NULL;

		send->s_sge[0].addr = ic->i_send_hdrs_dma
			+ (pos * sizeof(struct rds_header));
		send->s_sge[0].length = sizeof(struct rds_header);

		memcpy(&ic->i_send_hdrs[pos], &rm->m_inc.i_hdr, sizeof(struct rds_header));

		/* Set up the data, if present */
		if (i < work_alloc
		    && scat != &rm->data.op_sg[rm->data.op_count]) {
			len = min(RDS_FRAG_SIZE, ib_sg_dma_len(dev, scat) - off);
			send->s_wr.num_sge = 2;

			send->s_sge[1].addr = ib_sg_dma_address(dev, scat) + off;
			send->s_sge[1].length = len;

			bytes_sent += len;
			off += len;
			if (off == ib_sg_dma_len(dev, scat)) {
				scat++;
				off = 0;
			}
		}

		rds_ib_set_wr_signal_state(ic, send, 0);

		/*
		 * Always signal the last one if we're stopping due to flow control.
		 */
		if (ic->i_flowctl && flow_controlled && i == (work_alloc-1))
			send->s_wr.send_flags |= IB_SEND_SIGNALED | IB_SEND_SOLICITED;

		if (send->s_wr.send_flags & IB_SEND_SIGNALED)
			nr_sig++;

		rdsdebug("send %p wr %p num_sge %u next %p\n", send,
			 &send->s_wr, send->s_wr.num_sge, send->s_wr.next);

		if (ic->i_flowctl && adv_credits) {
			struct rds_header *hdr = &ic->i_send_hdrs[pos];

			/* add credit and redo the header checksum */
			hdr->h_credit = adv_credits;
			rds_message_make_checksum(hdr);
			adv_credits = 0;
			rds_ib_stats_inc(s_ib_tx_credit_updates);
		}

		if (prev)
			prev->s_wr.next = &send->s_wr;
		prev = send;

		pos = (pos + 1) % ic->i_send_ring.w_nr;
		send = &ic->i_sends[pos];
		i++;

	} while (i < work_alloc
		 && scat != &rm->data.op_sg[rm->data.op_count]);

	/* Account the RDS header in the number of bytes we sent, but just once.
	 * The caller has no concept of fragmentation. */
	if (hdr_off == 0)
		bytes_sent += sizeof(struct rds_header);

	/* if we finished the message then send completion owns it */
	if (scat == &rm->data.op_sg[rm->data.op_count]) {
		prev->s_op = ic->i_data_op;
		prev->s_wr.send_flags |= IB_SEND_SOLICITED;
		ic->i_data_op = NULL;
	}

	/* Put back wrs & credits we didn't use */
	if (i < work_alloc) {
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc - i);
		work_alloc = i;
	}
	if (ic->i_flowctl && i < credit_alloc)
		rds_ib_send_add_credits(conn, credit_alloc - i);

	if (nr_sig)
		atomic_add(nr_sig, &ic->i_signaled_sends);

	/* XXX need to worry about failed_wr and partial sends. */
	failed_wr = &first->s_wr;
	ret = ib_post_send(ic->i_cm_id->qp, &first->s_wr, &failed_wr);
	rdsdebug("ic %p first %p (wr %p) ret %d wr %p\n", ic,
		 first, &first->s_wr, ret, failed_wr);
	BUG_ON(failed_wr != &first->s_wr);
	if (ret) {
		printk(KERN_WARNING "RDS/IB: ib_post_send to %pI4 "
		       "returned %d\n", &conn->c_faddr, ret);
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_sub_signaled(ic, nr_sig);
		if (prev->s_op) {
			ic->i_data_op = prev->s_op;
			prev->s_op = NULL;
		}

		rds_ib_conn_error(ic->conn, "ib_post_send failed\n");
		goto out;
	}

	ret = bytes_sent;
out:
	BUG_ON(adv_credits);
	return ret;
printf("helloworld");
}
