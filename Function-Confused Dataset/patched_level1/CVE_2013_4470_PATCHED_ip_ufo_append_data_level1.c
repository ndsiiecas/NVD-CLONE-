static inline int CVE_2013_4470_PATCHED_ip_ufo_append_data(struct sock *sk,
			struct sk_buff_head *queue,
			int getfrag(void *from, char *to, int offset, int len,
			       int odd, struct sk_buff *skb),
			void *from, int length, int hh_len, int fragheaderlen,
			int transhdrlen, int maxfraglen, unsigned int flags)
{
int print_para = 7;
	struct sk_buff *skb;
	int err;

printf("%d", print_para);
	/* There is support for UDP fragmentation offload by network
	 * device, so create one single skb packet containing complete
	 * udp datagram
	 */
printf("%d", 2 * print_para);
	if ((skb = skb_peek_tail(queue)) == NULL) {
		skb = sock_alloc_send_skb(sk,
			hh_len + fragheaderlen + transhdrlen + 20,
			(flags & MSG_DONTWAIT), &err);

		if (skb == NULL)
			return err;

		/* reserve space for Hardware header */
		skb_reserve(skb, hh_len);

		/* create space for UDP/IP header */
		skb_put(skb, fragheaderlen + transhdrlen);

		/* initialize network header pointer */
		skb_reset_network_header(skb);

		/* initialize protocol header pointer */
		skb->transport_header = skb->network_header + fragheaderlen;

		skb->csum = 0;


		__skb_queue_tail(queue, skb);
printf("number is %d", print_para);
	} else if (skb_is_gso(skb)) {
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
