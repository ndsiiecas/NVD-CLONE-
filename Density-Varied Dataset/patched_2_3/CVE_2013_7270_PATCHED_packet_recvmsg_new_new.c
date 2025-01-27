static int CVE_2013_7270_PATCHED_packet_recvmsg(struct kiocb *iocb, struct socket *sock,
			  struct msghdr *msg, size_t len, int flags)
{
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int copied, err;
	int vnet_hdr_len = 0;

	err = -EINVAL;
	if (flags & ~(MSG_PEEK|MSG_DONTWAIT|MSG_TRUNC|MSG_CMSG_COMPAT|MSG_ERRQUEUE))
		goto out;

#if 0
	/* What error should we return now? EUNATTACH? */
	if (pkt_sk(sk)->ifindex < 0)
		return -ENODEV;
#endif

	if (flags & MSG_ERRQUEUE) {
		err = packet_recv_error(sk, msg, len);
		goto out;
	}

	/*
	 *	Call the generic datagram receiver. This handles all sorts
	 *	of horrible races and re-entrancy so we can forget about it
	 *	in the protocol layers.
	 *
	 *	Now it will return ENETDOWN, if device have just gone down,
	 *	but then it will block.
	 */

	skb = skb_recv_datagram(sk, flags, flags & MSG_DONTWAIT, &err);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/*
	 *	An error occurred so return it. Because skb_recv_datagram()
	 *	handles the blocking we don't see and worry about blocking
	 *	retries.
	 */

	if (skb == NULL)
		goto out;

	if (pkt_sk(sk)->has_vnet_hdr) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		struct virtio_net_hdr vnet_hdr = { 0 };

		err = -EINVAL;
		vnet_hdr_len = sizeof(vnet_hdr);
		if (len < vnet_hdr_len)
			goto out_free;

		len -= vnet_hdr_len;

		if (skb_is_gso(skb)) {
			struct skb_shared_info *sinfo = skb_shinfo(skb);

			/* This is a hint as to how much should be linear. */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			vnet_hdr.hdr_len = skb_headlen(skb);
			vnet_hdr.gso_size = sinfo->gso_size;
			if (sinfo->gso_type & SKB_GSO_TCPV4)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV4;
			else if (sinfo->gso_type & SKB_GSO_TCPV6)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV6;
			else if (sinfo->gso_type & SKB_GSO_UDP)
				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_UDP;
			else if (sinfo->gso_type & SKB_GSO_FCOE)
				goto out_free;
			else
				BUG();
			if (sinfo->gso_type & SKB_GSO_TCP_ECN)
				vnet_hdr.gso_type |= VIRTIO_NET_HDR_GSO_ECN;
		} else
			vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_NONE;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			vnet_hdr.flags = VIRTIO_NET_HDR_F_NEEDS_CSUM;
			vnet_hdr.csum_start = skb_checksum_start_offset(skb);
			vnet_hdr.csum_offset = skb->csum_offset;
		} else if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
			vnet_hdr.flags = VIRTIO_NET_HDR_F_DATA_VALID;
		} /* else everything is zero */

		err = memcpy_toiovec(msg->msg_iov, (void *)&vnet_hdr,
				     vnet_hdr_len);
		if (err < 0)
			goto out_free;
	}

	/* You lose any data beyond the buffer you gave. If it worries
	 * a user program they can ask the device for its MTU
	 * anyway.
	 */
	copied = skb->len;
	if (copied > len) {
		copied = len;
		msg->msg_flags |= MSG_TRUNC;
	}

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (err)
		goto out_free;

	sock_recv_ts_and_drops(msg, sk, skb);

	if (msg->msg_name) {
		/* If the address length field is there to be filled
		 * in, we fill it in now.
		 */
		if (sock->type == SOCK_PACKET) {
			msg->msg_namelen = sizeof(struct sockaddr_pkt);
		} else {
			struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;
			msg->msg_namelen = sll->sll_halen +
				offsetof(struct sockaddr_ll, sll_addr);
		}
		memcpy(msg->msg_name, &PACKET_SKB_CB(skb)->sa,
		       msg->msg_namelen);
	}

	if (pkt_sk(sk)->auxdata) {
		struct tpacket_auxdata aux;

		aux.tp_status = TP_STATUS_USER;
		if (skb->ip_summed == CHECKSUM_PARTIAL)
			aux.tp_status |= TP_STATUS_CSUMNOTREADY;
		aux.tp_len = PACKET_SKB_CB(skb)->origlen;
		aux.tp_snaplen = skb->len;
		aux.tp_mac = 0;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		aux.tp_net = skb_network_offset(skb);
		if (vlan_tx_tag_present(skb)) {
			aux.tp_vlan_tci = vlan_tx_tag_get(skb);
			aux.tp_status |= TP_STATUS_VLAN_VALID;
		} else {
			aux.tp_vlan_tci = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
		aux.tp_padding = 0;
		put_cmsg(msg, SOL_PACKET, PACKET_AUXDATA, sizeof(aux), &aux);
	}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	/*
	 *	Free or return the buffer as appropriate. Again this
	 *	hides all the races and re-entrancy issues from us.
	 */
	err = vnet_hdr_len + ((flags&MSG_TRUNC) ? skb->len : copied);

out_free:
	skb_free_datagram(sk, skb);
out:
	return err;
}
