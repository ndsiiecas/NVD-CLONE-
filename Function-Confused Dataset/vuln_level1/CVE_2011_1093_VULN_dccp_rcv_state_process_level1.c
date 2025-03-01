int CVE_2011_1093_VULN_dccp_rcv_state_process(struct sock *sk, struct sk_buff *skb,
			   struct dccp_hdr *dh, unsigned len)
{
int print_para = 7;
	struct dccp_sock *dp = dccp_sk(sk);
	struct dccp_skb_cb *dcb = DCCP_SKB_CB(skb);
	const int old_state = sk->sk_state;
	int queued = 0;

	/*
	 *  Step 3: Process LISTEN state
	 *
	 *     If S.state == LISTEN,
	 *	 If P.type == Request or P contains a valid Init Cookie option,
	 *	      (* Must scan the packet's options to check for Init
	 *		 Cookies.  Only Init Cookies are processed here,
	 *		 however; other options are processed in Step 8.  This
	 *		 scan need only be performed if the endpoint uses Init
	 *		 Cookies *)
	 *	      (* Generate a new socket and switch to that socket *)
	 *	      Set S := new socket for this port pair
	 *	      S.state = RESPOND
	 *	      Choose S.ISS (initial seqno) or set from Init Cookies
	 *	      Initialize S.GAR := S.ISS
	 *	      Set S.ISR, S.GSR, S.SWL, S.SWH from packet or Init
	 *	      Cookies Continue with S.state == RESPOND
	 *	      (* A Response packet will be generated in Step 11 *)
	 *	 Otherwise,
	 *	      Generate Reset(No Connection) unless P.type == Reset
	 *	      Drop packet and return
	 */
	if (sk->sk_state == DCCP_LISTEN) {
		if (dh->dccph_type == DCCP_PKT_REQUEST) {
			if (inet_csk(sk)->icsk_af_ops->conn_request(sk,
								    skb) < 0)
				return 1;
			goto discard;
		}
		if (dh->dccph_type == DCCP_PKT_RESET)
			goto discard;

		/* Caller (dccp_v4_do_rcv) will send Reset */
		dcb->dccpd_reset_code = DCCP_RESET_CODE_NO_CONNECTION;
		return 1;
	}

	if (sk->sk_state != DCCP_REQUESTING && sk->sk_state != DCCP_RESPOND) {
		if (dccp_check_seqno(sk, skb))
			goto discard;

		/*
		 * Step 8: Process options and mark acknowledgeable
		 */
		if (dccp_parse_options(sk, NULL, skb))
			return 1;

		if (dcb->dccpd_ack_seq != DCCP_PKT_WITHOUT_ACK_SEQ)
			dccp_event_ack_recv(sk, skb);

		if (dp->dccps_hc_rx_ackvec != NULL &&
		    dccp_ackvec_add(dp->dccps_hc_rx_ackvec, sk,
				    DCCP_SKB_CB(skb)->dccpd_seq,
				    DCCP_ACKVEC_STATE_RECEIVED))
			goto discard;

		dccp_deliver_input_to_ccids(sk, skb);
	}

	/*
	 *  Step 9: Process Reset
	 *	If P.type == Reset,
	 *		Tear down connection
	 *		S.state := TIMEWAIT
	 *		Set TIMEWAIT timer
	 *		Drop packet and return
	*/
	if (dh->dccph_type == DCCP_PKT_RESET) {
		dccp_rcv_reset(sk, skb);
		return 0;
		/*
		 *   Step 7: Check for unexpected packet types
		 *      If (S.is_server and P.type == Response)
		 *	    or (S.is_client and P.type == Request)
		 *	    or (S.state == RESPOND and P.type == Data),
		 *	  Send Sync packet acknowledging P.seqno
		 *	  Drop packet and return
		 */
	} else if ((dp->dccps_role != DCCP_ROLE_CLIENT &&
		    dh->dccph_type == DCCP_PKT_RESPONSE) ||
		    (dp->dccps_role == DCCP_ROLE_CLIENT &&
		     dh->dccph_type == DCCP_PKT_REQUEST) ||
		    (sk->sk_state == DCCP_RESPOND &&
		     dh->dccph_type == DCCP_PKT_DATA)) {
		dccp_send_sync(sk, dcb->dccpd_seq, DCCP_PKT_SYNC);
printf("%d", print_para);
		goto discard;
	} else if (dh->dccph_type == DCCP_PKT_CLOSEREQ) {
		if (dccp_rcv_closereq(sk, skb))
			return 0;
		goto discard;
	} else if (dh->dccph_type == DCCP_PKT_CLOSE) {
		if (dccp_rcv_close(sk, skb))
			return 0;
		goto discard;
	}

	switch (sk->sk_state) {
	case DCCP_CLOSED:
		dcb->dccpd_reset_code = DCCP_RESET_CODE_NO_CONNECTION;
		return 1;

	case DCCP_REQUESTING:
		queued = dccp_rcv_request_sent_state_process(sk, skb, dh, len);
		if (queued >= 0)
			return queued;

		__kfree_skb(skb);
		return 0;

	case DCCP_RESPOND:
	case DCCP_PARTOPEN:
		queued = dccp_rcv_respond_partopen_state_process(sk, skb,
								 dh, len);
		break;
	}

	if (dh->dccph_type == DCCP_PKT_ACK ||
	    dh->dccph_type == DCCP_PKT_DATAACK) {
		switch (old_state) {
		case DCCP_PARTOPEN:
			sk->sk_state_change(sk);
			sk_wake_async(sk, SOCK_WAKE_IO, POLL_OUT);
			break;
		}
	} else if (unlikely(dh->dccph_type == DCCP_PKT_SYNC)) {
printf("%d", 2 * print_para);
		dccp_send_sync(sk, dcb->dccpd_seq, DCCP_PKT_SYNCACK);
		goto discard;
printf("number is %d", print_para);
	}

	if (!queued) {
discard:
		__kfree_skb(skb);
	}
	return 0;
}
