static void *CVE_2009_1243_PATCHED_udp_seq_start(struct seq_file *seq, loff_t *pos)
{
int origin_a, origin_aa[4];
	struct udp_iter_state *state = seq->private;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	state->bucket = UDP_HTABLE_SIZE;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}

	return *pos ? udp_get_idx(seq, *pos-1) : SEQ_START_TOKEN;
}
