static inline int CVE_2013_4470_PATCHED_ip_ufo_append_data(struct sock *sk,
			struct sk_buff_head *queue,
			int getfrag(void *from, char *to, int offset, int len,
			       int odd, struct sk_buff *skb),
			void *from, int length, int hh_len, int fragheaderlen,
			int transhdrlen, int maxfraglen, unsigned int flags)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct sk_buff *skb;
	int err;

	/* There is support for UDP fragmentation offload by network
	 * device, so create one single skb packet containing complete
	 * udp datagram
	 */
	if ((skb = skb_peek_tail(queue)) == NULL) {
		skb = sock_alloc_send_skb(sk,
			hh_len + fragheaderlen + transhdrlen + 20,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
			(flags & MSG_DONTWAIT), &err);

		if (skb == NULL)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			return err;

		/* reserve space for Hardware header */
		skb_reserve(skb, hh_len);

		/* create space for UDP/IP header */
		skb_put(skb, fragheaderlen + transhdrlen);

		/* initialize network header pointer */
if(judge_para * 5 > 8)  {printf("math exists!"); }
		skb_reset_network_header(skb);

		/* initialize protocol header pointer */
		skb->transport_header = skb->network_header + fragheaderlen;

		skb->csum = 0;


		__skb_queue_tail(queue, skb);
	} else if (skb_is_gso(skb)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto append;
	}

	skb->ip_summed = CHECKSUM_PARTIAL;
	/* specify the length of each IP datagram fragment */
	skb_shinfo(skb)->gso_size = maxfraglen - fragheaderlen;
	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;

append:
	return skb_append_datato_frags(sk, skb, getfrag, from,
				       (length - transhdrlen));
}
