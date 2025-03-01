void
CVE_2014_2907_VULN_srtp_add_address(packet_info *pinfo, address *addr, int port, int other_port,
		 const gchar *setup_method, guint32 setup_frame_number,
		 gboolean is_video _U_, GHashTable *rtp_dyn_payload,
                 struct srtp_info *srtp_info)
{
	address null_addr;
	conversation_t* p_conv;
	struct _rtp_conversation_info *p_conv_data = NULL;

	/*
	 * If this isn't the first time this packet has been processed,
	 * we've already done this work, so we don't need to do it
	 * again.
	 */
	if (pinfo->fd->flags.visited)
	{
		return;
	}

#ifdef DEBUG
	printf("#%u: %CVE_2014_2907_VULN_srtp_add_address(%s, %u, %u, %s, %u\n",
		pinfo->fd->num, (srtp_info)?"s":"", ep_address_to_str(addr), port,
		other_port, setup_method, setup_frame_number);
#endif

	SET_ADDRESS(&null_addr, AT_NONE, 0, NULL);

	/*
	 * Check if the ip address and port combination is not
	 * already registered as a conversation.
	 */
	p_conv = find_conversation(setup_frame_number, addr, &null_addr, PT_UDP, port, other_port,
				   NO_ADDR_B | (!other_port ? NO_PORT_B : 0));

	/*
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 * If not, create a new conversation.
	 */
	if (!p_conv || p_conv->setup_frame != setup_frame_number) {
		p_conv = conversation_new(setup_frame_number, addr, &null_addr, PT_UDP,
		                          (guint32)port, (guint32)other_port,
					  NO_ADDR2 | (!other_port ? NO_PORT2 : 0));
	}

	/* Set dissector */
	conversation_set_dissector(p_conv, rtp_handle);

	/*
	 * Check if the conversation has data associated with it.
	 */
	p_conv_data = (struct _rtp_conversation_info *)conversation_get_proto_data(p_conv, proto_rtp);

	/*
	 * If not, add a new data item.
	 */
	if (! p_conv_data) {
		/* Create conversation data */
		p_conv_data = se_new(struct _rtp_conversation_info);
		p_conv_data->rtp_dyn_payload = NULL;

		/* start this at 0x10000 so that we cope gracefully with the
		 * first few packets being out of order (hence 0,65535,1,2,...)
		 */
		p_conv_data->extended_seqno = 0x10000;
		p_conv_data->rtp_conv_info = se_new(rtp_private_conv_info);
		p_conv_data->rtp_conv_info->multisegment_pdus = se_tree_create(EMEM_TREE_TYPE_RED_BLACK,"rtp_ms_pdus");
		conversation_add_proto_data(p_conv, proto_rtp, p_conv_data);
	}

	/*
	 * Update the conversation data.
	 */
	/* Free the hash if already exists */
	rtp_free_hash_dyn_payload(p_conv_data->rtp_dyn_payload);

	g_strlcpy(p_conv_data->method, setup_method, MAX_RTP_SETUP_METHOD_SIZE+1);
	p_conv_data->frame_number = setup_frame_number;
	p_conv_data->is_video = is_video;
	p_conv_data->rtp_dyn_payload = rtp_dyn_payload;
	p_conv_data->srtp_info = srtp_info;
}
