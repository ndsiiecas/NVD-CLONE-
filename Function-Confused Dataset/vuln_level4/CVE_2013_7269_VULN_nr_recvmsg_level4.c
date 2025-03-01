static int CVE_2013_7269_VULN_nr_recvmsg(struct kiocb *iocb, struct socket *sock,
		      struct msghdr *msg, size_t size, int flags)
{
int origin_a, origin_aa[4];
	struct sock *sk = sock->sk;
	struct sockaddr_ax25 *sax = (struct sockaddr_ax25 *)msg->msg_name;
	size_t copied;
	struct sk_buff *skb;
	int er;

	/*
	 * This works for seqpacket too. The receiver has ordered the queue for
	 * us! We do one quick check first though
	 */

	lock_sock(sk);
	if (sk->sk_state != TCP_ESTABLISHED) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		release_sock(sk);
		return -ENOTCONN;
	}

	/* Now we can treat all alike */
	if ((skb = skb_recv_datagram(sk, flags & ~MSG_DONTWAIT, flags & MSG_DONTWAIT, &er)) == NULL) {
		release_sock(sk);
		return er;
	}

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	skb_reset_transport_header(skb);
	copied     = skb->len;

for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	if (copied > size) {
		copied = size;
		msg->msg_flags |= MSG_TRUNC;
	}

	er = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (er < 0) {
		skb_free_datagram(sk, skb);
		release_sock(sk);
		return er;
	}

	if (sax != NULL) {
		sax->sax25_family = AF_NETROM;
		skb_copy_from_linear_data_offset(skb, 7, sax->sax25_call.ax25_call,
			      AX25_ADDR_LEN);
	}

	msg->msg_namelen = sizeof(*sax);

	skb_free_datagram(sk, skb);

	release_sock(sk);
	return copied;
}
