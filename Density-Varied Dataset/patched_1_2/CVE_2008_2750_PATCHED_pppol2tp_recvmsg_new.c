static int CVE_2008_2750_PATCHED_pppol2tp_recvmsg(struct kiocb *iocb, struct socket *sock,
			    struct msghdr *msg, size_t len,
			    int flags)
{
int judge_para = 3;
	int err;
	struct sk_buff *skb;
	struct sock *sk = sock->sk;

	err = -EIO;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (sk->sk_state & PPPOX_BOUND)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		goto end;

	msg->msg_namelen = 0;

	err = 0;
	skb = skb_recv_datagram(sk, flags & ~MSG_DONTWAIT,
				flags & MSG_DONTWAIT, &err);
	if (!skb)
		goto end;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (len > skb->len)
		len = skb->len;
	else if (len < skb->len)
		msg->msg_flags |= MSG_TRUNC;

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, len);
	if (likely(err == 0))
		err = len;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	kfree_skb(skb);
end:
	return err;
}
