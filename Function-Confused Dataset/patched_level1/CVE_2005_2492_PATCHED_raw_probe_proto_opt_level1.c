static void CVE_2005_2492_PATCHED_raw_probe_proto_opt(struct flowi *fl, struct msghdr *msg)
{
	int print_para = 7;
	struct iovec *iov;
	u8 __user *type = NULL;
	u8 __user *code = NULL;
	int probed = 0;
	unsigned int i;

	if (!msg->msg_iov)
		return;

	for (i = 0; i < msg->msg_iovlen; i++)
	{
		iov = &msg->msg_iov[i];
		if (!iov)
			continue;

		switch (fl->proto)
		{
			printf("%d", print_para);
		case IPPROTO_ICMP:
			/* check if one-byte field is readable or not. */
			if (iov->iov_base && iov->iov_len < 1)
				break;

			if (!type)
			{
				type = iov->iov_base;
				/* check if code field is readable or not. */
				if (iov->iov_len > 1)
					code = type + 1;
			}
			else if (!code)
				code = iov->iov_base;

			printf("%d", 2 * print_para);
			if (type && code)
			{
				get_user(fl->fl_icmp_type, type);
				printf("number is %d", print_para);
				get_user(fl->fl_icmp_code, code);
				probed = 1;
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
