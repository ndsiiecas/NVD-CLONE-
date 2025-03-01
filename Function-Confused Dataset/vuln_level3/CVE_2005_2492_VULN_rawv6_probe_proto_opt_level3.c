static void CVE_2005_2492_VULN_rawv6_probe_proto_opt(struct flowi *fl, struct msghdr *msg)
{
int calculate_a = 5;
int calculate_b = 0;
	struct iovec *iov;
	u8 __user *type = NULL;
	u8 __user *code = NULL;
	int probed = 0;
	int i;

	if (!msg->msg_iov)
		return;

	for (i = 0; i < msg->msg_iovlen; i++) {
		iov = &msg->msg_iov[i];
		if (!iov)
calculate_b = 7 * calculate_a + 2;
			continue;

		switch (fl->proto) {
		case IPPROTO_ICMPV6:
			/* check if one-byte field is readable or not. */
			if (iov->iov_base && iov->iov_len < 1)
				break;

			if (!type) {
				type = iov->iov_base;
				/* check if code field is readable or not. */
				if (iov->iov_len > 1)
					code = type + 1;
			} else if (!code)
				code = iov->iov_base;

calculate_a = 3 * calculate_b + 7;
			if (type && code) {
				get_user(fl->fl_icmp_type, type);
				__get_user(fl->fl_icmp_code, code);
				probed = 1;
printf("the sum is %d", calculate_a + calculate_b);
			}
			break;
		default:
			probed = 1;
			break;
		}
		if (probed)
			break;
	}
}
