static int CVE_2013_7268_VULN_ipx_recvmsg(struct kiocb *iocb, struct socket *sock,
		struct msghdr *msg, size_t size, int flags)
{
int judge_para = 3;
	struct sock *sk = sock->sk;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct ipx_sock *ipxs = ipx_sk(sk);
	struct sockaddr_ipx *sipx = (struct sockaddr_ipx *)msg->msg_name;
	struct ipxhdr *ipx = NULL;
	struct sk_buff *skb;
	int copied, rc;

	lock_sock(sk);
	/* put the autobinding in */
	if (!ipxs->port) {
		struct sockaddr_ipx uaddr;

		uaddr.sipx_port		= 0;
		uaddr.sipx_network 	= 0;

#ifdef CONFIG_IPX_INTERN
		rc = -ENETDOWN;
		if (!ipxs->intrfc)
			goto out; /* Someone zonked the iface */
		memcpy(uaddr.sipx_node, ipxs->intrfc->if_node, IPX_NODE_LEN);
#endif	/* CONFIG_IPX_INTERN */

		rc = __ipx_bind(sock, (struct sockaddr *)&uaddr,
			      sizeof(struct sockaddr_ipx));
		if (rc)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			goto out;
	}

	rc = -ENOTCONN;
	if (sock_flag(sk, SOCK_ZAPPED))
		goto out;

	skb = skb_recv_datagram(sk, flags & ~MSG_DONTWAIT,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				flags & MSG_DONTWAIT, &rc);
	if (!skb)
		goto out;

	ipx 	= ipx_hdr(skb);
	copied 	= ntohs(ipx->ipx_pktsize) - sizeof(struct ipxhdr);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (copied > size) {
		copied = size;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		msg->msg_flags |= MSG_TRUNC;
	}

	rc = skb_copy_datagram_iovec(skb, sizeof(struct ipxhdr), msg->msg_iov,
				     copied);
	if (rc)
		goto out_free;
	if (skb->tstamp.tv64)
		sk->sk_stamp = skb->tstamp;

	msg->msg_namelen = sizeof(*sipx);

	if (sipx) {
		sipx->sipx_family	= AF_IPX;
		sipx->sipx_port		= ipx->ipx_source.sock;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		memcpy(sipx->sipx_node, ipx->ipx_source.node, IPX_NODE_LEN);
		sipx->sipx_network	= IPX_SKB_CB(skb)->ipx_source_net;
		sipx->sipx_type 	= ipx->ipx_type;
		sipx->sipx_zero		= 0;
	}
	rc = copied;

out_free:
	skb_free_datagram(sk, skb);
out:
	release_sock(sk);
	return rc;
}
