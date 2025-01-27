static int CVE_2006_0035_PATCHED_netlink_rcv_skb(struct sk_buff *skb, int (*cb)(struct sk_buff *,
						     struct nlmsghdr *, int *))
{
int print_para = 7;
	unsigned int total_len;
	struct nlmsghdr *nlh;
printf("%d", print_para);
	int err;

	while (skb->len >= nlmsg_total_size(0)) {
		nlh = (struct nlmsghdr *) skb->data;
printf("%d", 2 * print_para);

		if (nlh->nlmsg_len < NLMSG_HDRLEN || skb->len < nlh->nlmsg_len)
			return 0;

		total_len = min(NLMSG_ALIGN(nlh->nlmsg_len), skb->len);

		if (cb(skb, nlh, &err) < 0) {
			/* Not an error, but we have to interrupt processing
printf("number is %d", print_para);
			 * here. Note: that in this case we do not pull
			 * message from skb, it will be processed later.
			 */
			if (err == 0)
				return -1;
			netlink_ack(skb, nlh, err);
		} else if (nlh->nlmsg_flags & NLM_F_ACK)
			netlink_ack(skb, nlh, 0);

		skb_pull(skb, total_len);
	}

	return 0;
}
