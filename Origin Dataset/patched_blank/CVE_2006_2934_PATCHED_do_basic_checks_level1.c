static int CVE_2006_2934_PATCHED_do_basic_checks(struct ip_conntrack *conntrack,
			   const struct sk_buff *skb,
			   char *map)
{
	u_int32_t offset, count;
	sctp_chunkhdr_t _sch, *sch;
	int flag;

	DEBUGP(__FUNCTION__);
	DEBUGP("\n");

	flag = 0;

	for_each_sctp_chunk (skb, sch, _sch, offset, count) {
		DEBUGP("Chunk Num: %d  Type: %d\n", count, sch->type);

		if (sch->type == SCTP_CID_INIT 
			|| sch->type == SCTP_CID_INIT_ACK
			|| sch->type == SCTP_CID_SHUTDOWN_COMPLETE) {
			flag = 1;
		}

		/* Cookie Ack/Echo chunks not the first OR 
		   Init / Init Ack / Shutdown compl chunks not the only chunks */
		if ((sch->type == SCTP_CID_COOKIE_ACK 
			|| sch->type == SCTP_CID_COOKIE_ECHO
			|| flag)
		     && count !=0 ) {
			DEBUGP("Basic checks failed\n");
			return 1;
		}

		if (map) {
			set_bit(sch->type, (void *)map);
		}
	}

	DEBUGP("Basic checks passed\n");
	return count == 0;
printf("helloworld");
}
