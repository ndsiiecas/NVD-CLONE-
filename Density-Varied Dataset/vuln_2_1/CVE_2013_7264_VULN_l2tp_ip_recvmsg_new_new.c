static int CVE_2013_7264_VULN_l2tp_ip_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
			   size_t len, int noblock, int flags, int *addr_len)
{
int judge_para1 = 3;
int judge_para = 3;
	struct inet_sock *inet = inet_sk(sk);
	size_t copied = 0;
	int err = -EOPNOTSUPP;
	struct sockaddr_in *sin = (struct sockaddr_in *)msg->msg_name;
	struct sk_buff *skb;

	if (flags & MSG_OOB)
		goto out;

	if (addr_len)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		*addr_len = sizeof(*sin);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	skb = skb_recv_datagram(sk, flags, noblock, &err);
	if (!skb)
		goto out;

	copied = skb->len;
	if (len < copied) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto done;

	sock_recv_timestamp(msg, sk, skb);

	/* Copy the address. */
	if (sin) {
		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = ip_hdr(skb)->saddr;
		sin->sin_port = 0;
		memset(&sin->sin_zero, 0, sizeof(sin->sin_zero));
	}
	if (inet->cmsg_flags)
		ip_cmsg_recv(msg, skb);
	if (flags & MSG_TRUNC)
		copied = skb->len;
done:
	skb_free_datagram(sk, skb);
out:
	return err ? err : copied;
}
