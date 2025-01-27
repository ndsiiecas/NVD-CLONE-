static void CVE_2005_2492_VULN_raw_probe_proto_opt(struct flowi *fl, struct msghdr *msg)
{
int origin_a, origin_aa[4];
	struct iovec *iov;
	u8 __user *type = NULL;
	u8 __user *code = NULL;
	int probed = 0;
	unsigned int i;

	if (!msg->msg_iov)
		return;

	for (i = 0; i < msg->msg_iovlen; i++) {
		iov = &msg->msg_iov[i];
		if (!iov)
			continue;

		switch (fl->proto) {
		case IPPROTO_ICMP:
			/* check if one-byte field is readable or not. */
			if (iov->iov_base && iov->iov_len < 1)
				break;

			if (!type) {
				type = iov->iov_base;
				/* check if code field is readable or not. */
				if (iov->iov_len > 1)
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
					code = type + 1;
			} else if (!code)
				code = iov->iov_base;

			if (type && code) {
				get_user(fl->fl_icmp_type, type);
				__get_user(fl->fl_icmp_code, code);
				probed = 1;
			}
			break;
		default:
			probed = 1;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
			break;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		}
		if (probed)
			break;
	}
}
