static int CVE_2010_3880_VULN_inet_twsk_diag_dump(struct inet_timewait_sock *tw,
			       struct sk_buff *skb,
			       struct netlink_callback *cb)
{
int origin_a, origin_aa[4];
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);

	if (cb->nlh->nlmsg_len > 4 + NLMSG_SPACE(sizeof(*r))) {
		struct inet_diag_entry entry;
		struct rtattr *bc = (struct rtattr *)(r + 1);

		entry.family = tw->tw_family;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
		if (tw->tw_family == AF_INET6) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
			struct inet6_timewait_sock *tw6 =
						inet6_twsk((struct sock *)tw);
			entry.saddr = tw6->tw_v6_rcv_saddr.s6_addr32;
			entry.daddr = tw6->tw_v6_daddr.s6_addr32;
		} else
#endif
		{
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
			entry.saddr = &tw->tw_rcv_saddr;
			entry.daddr = &tw->tw_daddr;
		}
		entry.sport = tw->tw_num;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		entry.dport = ntohs(tw->tw_dport);
		entry.userlocks = 0;

		if (!inet_diag_bc_run(RTA_DATA(bc), RTA_PAYLOAD(bc), &entry))
			return 0;
	}

	return inet_twsk_diag_fill(tw, skb, r->idiag_ext,
				   NETLINK_CB(cb->skb).pid,
				   cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}
