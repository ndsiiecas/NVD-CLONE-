static int CVE_2013_7281_VULN_dgram_recvmsg(struct kiocb *iocb, struct sock *sk,
		struct msghdr *msg, size_t len, int noblock, int flags,
		int *addr_len)
{
int judge_para = 3;
	size_t copied = 0;
	int err = -EOPNOTSUPP;
	struct sk_buff *skb;
	struct sockaddr_ieee802154 *saddr;

	saddr = (struct sockaddr_ieee802154 *)msg->msg_name;

	skb = skb_recv_datagram(sk, flags, noblock, &err);
	if (!skb)
		goto out;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	copied = skb->len;
	if (len < copied) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}

	/* FIXME: skip headers if necessary ?! */
	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto done;

	sock_recv_ts_and_drops(msg, sk, skb);

	if (saddr) {
		saddr->family = AF_IEEE802154;
		saddr->addr = mac_cb(skb)->sa;
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (addr_len)
		*addr_len = sizeof(*saddr);

	if (flags & MSG_TRUNC)
		copied = skb->len;
done:
	skb_free_datagram(sk, skb);
out:
	if (err)
		return err;
	return copied;
}
