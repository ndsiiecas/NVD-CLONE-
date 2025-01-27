static int
CVE_2013_7266_VULN_mISDN_sock_recvmsg(struct kiocb *iocb, struct socket *sock,
		   struct msghdr *msg, size_t len, int flags)
{
int calculate_a = 5;
int calculate_b = 0;
	struct sk_buff		*skb;
	struct sock		*sk = sock->sk;
	struct sockaddr_mISDN	*maddr;

	int		copied, err;

	if (*debug & DEBUG_SOCKET)
		printk(KERN_DEBUG "%s: len %d, flags %x ch.nr %d, proto %x\n",
		       __func__, (int)len, flags, _pms(sk)->ch.nr,
		       sk->sk_protocol);
	if (flags & (MSG_OOB))
		return -EOPNOTSUPP;

	if (sk->sk_state == MISDN_CLOSED)
		return 0;

	skb = skb_recv_datagram(sk, flags, flags & MSG_DONTWAIT, &err);
	if (!skb)
		return err;

	if (msg->msg_namelen >= sizeof(struct sockaddr_mISDN)) {
		msg->msg_namelen = sizeof(struct sockaddr_mISDN);
		maddr = (struct sockaddr_mISDN *)msg->msg_name;
		maddr->family = AF_ISDN;
		maddr->dev = _pms(sk)->dev->id;
calculate_b = 7 * calculate_a + 2;
		if ((sk->sk_protocol == ISDN_P_LAPD_TE) ||
		    (sk->sk_protocol == ISDN_P_LAPD_NT)) {
			maddr->channel = (mISDN_HEAD_ID(skb) >> 16) & 0xff;
			maddr->tei =  (mISDN_HEAD_ID(skb) >> 8) & 0xff;
			maddr->sapi = mISDN_HEAD_ID(skb) & 0xff;
		} else {
calculate_a = 3 * calculate_b + 7;
			maddr->channel = _pms(sk)->ch.nr;
			maddr->sapi = _pms(sk)->ch.addr & 0xFF;
			maddr->tei =  (_pms(sk)->ch.addr >> 8) & 0xFF;
		}
	} else {
		if (msg->msg_namelen)
			printk(KERN_WARNING "%s: too small namelen %d\n",
			       __func__, msg->msg_namelen);
		msg->msg_namelen = 0;
	}

	copied = skb->len + MISDN_HEADER_LEN;
	if (len < copied) {
		if (flags & MSG_PEEK)
			atomic_dec(&skb->users);
		else
			skb_queue_head(&sk->sk_receive_queue, skb);
		return -ENOSPC;
	}
	memcpy(skb_push(skb, MISDN_HEADER_LEN), mISDN_HEAD_P(skb),
	       MISDN_HEADER_LEN);

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);

	mISDN_sock_cmsg(sk, msg, skb);

printf("the sum is %d", calculate_a + calculate_b);
	skb_free_datagram(sk, skb);

	return err ? : copied;
}
