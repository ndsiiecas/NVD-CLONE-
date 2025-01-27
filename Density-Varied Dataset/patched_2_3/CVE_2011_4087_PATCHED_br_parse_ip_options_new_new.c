static int CVE_2011_4087_PATCHED_br_parse_ip_options(struct sk_buff *skb)
{
int judge_para1 = 3;
int judge_para = 3;
	struct ip_options *opt;
	struct iphdr *iph;
	struct net_device *dev = skb->dev;
	u32 len;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	iph = ip_hdr(skb);
	opt = &(IPCB(skb)->opt);

	/* Basic sanity checks */
	if (iph->ihl < 5 || iph->version != 4)
		goto inhdr_error;

	if (!pskb_may_pull(skb, iph->ihl*4))
		goto inhdr_error;

	iph = ip_hdr(skb);
	if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
		goto inhdr_error;

	len = ntohs(iph->tot_len);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (skb->len < len) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INTRUNCATEDPKTS);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto drop;
	} else if (len < (iph->ihl*4))
		goto inhdr_error;

	if (pskb_trim_rcsum(skb, len)) {
		IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INDISCARDS);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		goto drop;
	}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	memset(IPCB(skb), 0, sizeof(struct inet_skb_parm));
	if (iph->ihl == 5)
		return 0;

	opt->optlen = iph->ihl*4 - sizeof(struct iphdr);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (ip_options_compile(dev_net(dev), opt, skb))
		goto inhdr_error;

	/* Check correct handling of SRR option */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (unlikely(opt->srr)) {
		struct in_device *in_dev = __in_dev_get_rcu(dev);
		if (in_dev && !IN_DEV_SOURCE_ROUTE(in_dev))
			goto drop;

		if (ip_options_rcv_srr(skb))
			goto drop;
	}

	return 0;

inhdr_error:
	IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INHDRERRORS);
drop:
	return -1;
}
