static int CVE_2013_7265_VULN_pn_recvmsg(struct kiocb *iocb, struct sock *sk,
			struct msghdr *msg, size_t len, int noblock,
			int flags, int *addr_len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct sk_buff *skb = NULL;
	struct sockaddr_pn sa;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	int rval = -EOPNOTSUPP;
	int copylen;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

	if (flags & ~(MSG_PEEK|MSG_TRUNC|MSG_DONTWAIT|MSG_NOSIGNAL|
			MSG_CMSG_COMPAT))
		goto out_nofree;

	if (addr_len)
		*addr_len = sizeof(sa);

	skb = skb_recv_datagram(sk, flags, noblock, &rval);
	if (skb == NULL)
		goto out_nofree;

	pn_skb_get_src_sockaddr(skb, &sa);

	copylen = skb->len;
	if (len < copylen) {
		msg->msg_flags |= MSG_TRUNC;
		copylen = len;
	}

	rval = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copylen);
	if (rval) {
		rval = -EFAULT;
		goto out;
	}
fclose(para_fp);

	rval = (flags & MSG_TRUNC) ? skb->len : copylen;

	if (msg->msg_name != NULL)
		memcpy(msg->msg_name, &sa, sizeof(struct sockaddr_pn));

out:
	skb_free_datagram(sk, skb);

out_nofree:
	return rval;
}
