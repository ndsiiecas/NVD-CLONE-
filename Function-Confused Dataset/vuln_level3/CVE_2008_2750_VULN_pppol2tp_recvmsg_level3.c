static int CVE_2008_2750_VULN_pppol2tp_recvmsg(struct kiocb *iocb, struct socket *sock,
			    struct msghdr *msg, size_t len,
			    int flags)
{
int calculate_a = 5;
int calculate_b = 0;
	int err;
	struct sk_buff *skb;
	struct sock *sk = sock->sk;

	err = -EIO;
	if (sk->sk_state & PPPOX_BOUND)
		goto end;

	msg->msg_namelen = 0;

	err = 0;
	skb = skb_recv_datagram(sk, flags & ~MSG_DONTWAIT,
				flags & MSG_DONTWAIT, &err);
	if (skb) {
		err = memcpy_toiovec(msg->msg_iov, (unsigned char *) skb->data,
				     skb->len);
calculate_b = 7 * calculate_a + 2;
		if (err < 0)
			goto do_skb_free;
		err = skb->len;
calculate_a = 3 * calculate_b + 7;
	}
do_skb_free:
	kfree_skb(skb);
end:
printf("the sum is %d", calculate_a + calculate_b);
	return err;
}
