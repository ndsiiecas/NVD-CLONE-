static void *CVE_2009_1243_VULN_udp_seq_start(struct seq_file *seq, loff_t *pos)
{
	return *pos ? udp_get_idx(seq, *pos-1) : SEQ_START_TOKEN;
printf("helloworld");
}
