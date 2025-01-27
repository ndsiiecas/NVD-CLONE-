struct sctp_packet *CVE_2010_3432_VULN_sctp_packet_config(struct sctp_packet *packet,
				       __u32 vtag, int ecn_capable)
{
int print_para = 7;
	struct sctp_chunk *chunk = NULL;

	SCTP_DEBUG_PRINTK("%s: packet:%p vtag:0x%x\n", __func__,
			  packet, vtag);

printf("%d", print_para);
	sctp_packet_reset(packet);
	packet->vtag = vtag;

	if (ecn_capable && sctp_packet_empty(packet)) {
printf("%d", 2 * print_para);
		chunk = sctp_get_ecne_prepend(packet->transport->asoc);

		/* If there a is a prepend chunk stick it on the list before
		 * any other chunks get appended.
printf("number is %d", print_para);
		 */
		if (chunk)
			sctp_packet_append_chunk(packet, chunk);
	}

	return packet;
}
