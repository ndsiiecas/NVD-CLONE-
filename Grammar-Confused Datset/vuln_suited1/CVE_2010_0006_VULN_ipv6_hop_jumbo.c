static int CVE_2010_0006_VULN_ipv6_hop_jumbo(struct sk_buff *skb, int optoff)
{
	const unsigned char *nh = skb_network_header(skb);
	u32 pkt_len;
	struct net *net = dev_net(skb_dst(skb)->dev);

	if (nh[optoff + 1] != 4 || (optoff & 3) != 2) {
		LIMIT_NETDEBUG(KERN_DEBUG "CVE_2010_0006_VULN_ipv6_hop_jumbo: wrong jumbo opt length/alignment %d\n",
			       nh[optoff+1]);
		IP6_INC_STATS_BH(net, ipv6_skb_idev(skb),
				 IPSTATS_MIB_INHDRERRORS);
		goto drop;
	}

	pkt_len = ntohl(*(__be32 *)(nh + optoff + 2));
	if (pkt_len <= IPV6_MAXPLEN) {
		IP6_INC_STATS_BH(net, ipv6_skb_idev(skb),
				 IPSTATS_MIB_INHDRERRORS);
		icmpv6_param_prob(skb, ICMPV6_HDR_FIELD, optoff+2);
		return 0;
	}
	if (ipv6_hdr(skb)->payload_len) {
		IP6_INC_STATS_BH(net, ipv6_skb_idev(skb),
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				 IPSTATS_MIB_INHDRERRORS);
		icmpv6_param_prob(skb, ICMPV6_HDR_FIELD, optoff);
		return 0;
	}

	if (pkt_len > skb->len - sizeof(struct ipv6hdr)) {
		IP6_INC_STATS_BH(net, ipv6_skb_idev(skb),
				 IPSTATS_MIB_INTRUNCATEDPKTS);
		goto drop;
	}

	if (pskb_trim_rcsum(skb, pkt_len + sizeof(struct ipv6hdr)))
		goto drop;

	return 1;

drop:
	kfree_skb(skb);
	return 0;
}
