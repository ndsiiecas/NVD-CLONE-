static int
CVE_2013_7266_PATCHED_mISDN_sock_recvmsg(struct kiocb *iocb, struct socket *sock,
		   struct msghdr *msg, size_t len, int flags)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sk_buff		*skb;
	struct sock		*sk = sock->sk;

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

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (msg->msg_name) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		struct sockaddr_mISDN *maddr = msg->msg_name;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

		maddr->family = AF_ISDN;
		maddr->dev = _pms(sk)->dev->id;
		if ((sk->sk_protocol == ISDN_P_LAPD_TE) ||
		    (sk->sk_protocol == ISDN_P_LAPD_NT)) {
			maddr->channel = (mISDN_HEAD_ID(skb) >> 16) & 0xff;
			maddr->tei =  (mISDN_HEAD_ID(skb) >> 8) & 0xff;
			maddr->sapi = mISDN_HEAD_ID(skb) & 0xff;
		} else {
			maddr->channel = _pms(sk)->ch.nr;
			maddr->sapi = _pms(sk)->ch.addr & 0xFF;
			maddr->tei =  (_pms(sk)->ch.addr >> 8) & 0xFF;
		}
		msg->msg_namelen = sizeof(*maddr);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	copied = skb->len + MISDN_HEADER_LEN;
	if (len < copied) {
		if (flags & MSG_PEEK)
			atomic_dec(&skb->users);
		else
			skb_queue_head(&sk->sk_receive_queue, skb);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return -ENOSPC;
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	memcpy(skb_push(skb, MISDN_HEADER_LEN), mISDN_HEAD_P(skb),
	       MISDN_HEADER_LEN);

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);

	mISDN_sock_cmsg(sk, msg, skb);

	skb_free_datagram(sk, skb);

	return err ? : copied;
}
