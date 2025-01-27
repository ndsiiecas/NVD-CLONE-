static int CVE_2013_7271_PATCHED_x25_recvmsg(struct kiocb *iocb, struct socket *sock,
		       struct msghdr *msg, size_t size,
		       int flags)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct x25_sock *x25 = x25_sk(sk);
	struct sockaddr_x25 *sx25 = (struct sockaddr_x25 *)msg->msg_name;
	size_t copied;
	int qbit, header_len;
	struct sk_buff *skb;
	unsigned char *asmptr;
	int rc = -ENOTCONN;

	lock_sock(sk);

	if (x25->neighbour == NULL)
		goto out;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	header_len = x25->neighbour->extended ?
		X25_EXT_MIN_LEN : X25_STD_MIN_LEN;

	/*
	 * This works for seqpacket too. The receiver has ordered the queue for
	 * us! We do one quick check first though
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (sk->sk_state != TCP_ESTABLISHED)
		goto out;

	if (flags & MSG_OOB) {
		rc = -EINVAL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (sock_flag(sk, SOCK_URGINLINE) ||
		    !skb_peek(&x25->interrupt_in_queue))
			goto out;

		skb = skb_dequeue(&x25->interrupt_in_queue);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

		if (!pskb_may_pull(skb, X25_STD_MIN_LEN))
			goto out_free_dgram;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

		skb_pull(skb, X25_STD_MIN_LEN);

		/*
		 *	No Q bit information on Interrupt data.
		 */
		if (test_bit(X25_Q_BIT_FLAG, &x25->flags)) {
			asmptr  = skb_push(skb, 1);
			*asmptr = 0x00;
		}

		msg->msg_flags |= MSG_OOB;
	} else {
		/* Now we can treat all alike */
		release_sock(sk);
		skb = skb_recv_datagram(sk, flags & ~MSG_DONTWAIT,
					flags & MSG_DONTWAIT, &rc);
		lock_sock(sk);
		if (!skb)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			goto out;

		if (!pskb_may_pull(skb, header_len))
			goto out_free_dgram;

		qbit = (skb->data[0] & X25_Q_BIT) == X25_Q_BIT;

		skb_pull(skb, header_len);

		if (test_bit(X25_Q_BIT_FLAG, &x25->flags)) {
			asmptr  = skb_push(skb, 1);
			*asmptr = qbit;
		}
	}

	skb_reset_transport_header(skb);
	copied = skb->len;

	if (copied > size) {
		copied = size;
		msg->msg_flags |= MSG_TRUNC;
	}

	/* Currently, each datagram always contains a complete record */
	msg->msg_flags |= MSG_EOR;

	rc = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	if (rc)
		goto out_free_dgram;

	if (sx25) {
		sx25->sx25_family = AF_X25;
		sx25->sx25_addr   = x25->dest_addr;
		msg->msg_namelen = sizeof(*sx25);
	}

	x25_check_rbuf(sk);
	rc = copied;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out_free_dgram:
	skb_free_datagram(sk, skb);
out:
	release_sock(sk);
	return rc;
}
