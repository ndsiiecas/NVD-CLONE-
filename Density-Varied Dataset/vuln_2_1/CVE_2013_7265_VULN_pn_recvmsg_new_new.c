static int CVE_2013_7265_VULN_pn_recvmsg(struct kiocb *iocb, struct sock *sk,
			struct msghdr *msg, size_t len, int noblock,
			int flags, int *addr_len)
{
int judge_para1 = 3;
int judge_para = 3;
	struct sk_buff *skb = NULL;
	struct sockaddr_pn sa;
	int rval = -EOPNOTSUPP;
	int copylen;

	if (flags & ~(MSG_PEEK|MSG_TRUNC|MSG_DONTWAIT|MSG_NOSIGNAL|
			MSG_CMSG_COMPAT))
		goto out_nofree;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	if (addr_len)
		*addr_len = sizeof(sa);

	skb = skb_recv_datagram(sk, flags, noblock, &rval);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (skb == NULL)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		goto out_nofree;

	pn_skb_get_src_sockaddr(skb, &sa);

	copylen = skb->len;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (len < copylen) {
		msg->msg_flags |= MSG_TRUNC;
		copylen = len;
	}

	rval = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copylen);
	if (rval) {
		rval = -EFAULT;
		goto out;
	}

	rval = (flags & MSG_TRUNC) ? skb->len : copylen;

	if (msg->msg_name != NULL)
		memcpy(msg->msg_name, &sa, sizeof(struct sockaddr_pn));

out:
	skb_free_datagram(sk, skb);

out_nofree:
	return rval;
}
