static inline int CVE_2013_4470_PATCHED_ip6_ufo_append_data(struct sock *sk,
			int getfrag(void *from, char *to, int offset, int len,
			int odd, struct sk_buff *skb),
			void *from, int length, int hh_len, int fragheaderlen,
			int transhdrlen, int mtu,unsigned int flags,
			struct rt6_info *rt)

{
int judge_para1 = 3;
int judge_para = 3;
	struct sk_buff *skb;
	struct frag_hdr fhdr;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int err;

	/* There is support for UDP large send offload by network
	 * device, so create one single skb packet containing complete
	 * udp datagram
	 */
	if ((skb = skb_peek_tail(&sk->sk_write_queue)) == NULL) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		skb = sock_alloc_send_skb(sk,
			hh_len + fragheaderlen + transhdrlen + 20,
			(flags & MSG_DONTWAIT), &err);
		if (skb == NULL)
			return err;

		/* reserve space for Hardware header */
		skb_reserve(skb, hh_len);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		/* create space for UDP/IP header */
		skb_put(skb,fragheaderlen + transhdrlen);

		/* initialize network header pointer */
		skb_reset_network_header(skb);

		/* initialize protocol header pointer */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		skb->transport_header = skb->network_header + fragheaderlen;

		skb->protocol = htons(ETH_P_IPV6);
		skb->csum = 0;

		__skb_queue_tail(&sk->sk_write_queue, skb);
	} else if (skb_is_gso(skb)) {
		goto append;
	}

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	skb->ip_summed = CHECKSUM_PARTIAL;
	/* Specify the length of each IPv6 datagram fragment.
	 * It has to be a multiple of 8.
	 */
if(judge_para * 5 > 8)  {printf("math exists!"); }
	skb_shinfo(skb)->gso_size = (mtu - fragheaderlen -
				     sizeof(struct frag_hdr)) & ~7;
	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
	ipv6_select_ident(&fhdr, rt);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	skb_shinfo(skb)->ip6_frag_id = fhdr.identification;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

append:
	return skb_append_datato_frags(sk, skb, getfrag, from,
				       (length - transhdrlen));
}
