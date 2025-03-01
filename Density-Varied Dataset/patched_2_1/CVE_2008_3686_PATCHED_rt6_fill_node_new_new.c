static int CVE_2008_3686_PATCHED_rt6_fill_node(struct sk_buff *skb, struct rt6_info *rt,
			 struct in6_addr *dst, struct in6_addr *src,
			 int iif, int type, u32 pid, u32 seq,
			 int prefix, int nowait, unsigned int flags)
{
int judge_para1 = 3;
int judge_para = 3;
	struct rtmsg *rtm;
	struct nlmsghdr *nlh;
	long expires;
	u32 table;

	if (prefix) {	/* user wants prefix routes only */
		if (!(rt->rt6i_flags & RTF_PREFIX_RT)) {
			/* success since this is not a prefix route */
			return 1;
		}
	}

	nlh = nlmsg_put(skb, pid, seq, type, sizeof(*rtm), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	rtm = nlmsg_data(nlh);
	rtm->rtm_family = AF_INET6;
	rtm->rtm_dst_len = rt->rt6i_dst.plen;
	rtm->rtm_src_len = rt->rt6i_src.plen;
	rtm->rtm_tos = 0;
	if (rt->rt6i_table)
		table = rt->rt6i_table->tb6_id;
	else
		table = RT6_TABLE_UNSPEC;
	rtm->rtm_table = table;
	NLA_PUT_U32(skb, RTA_TABLE, table);
	if (rt->rt6i_flags&RTF_REJECT)
		rtm->rtm_type = RTN_UNREACHABLE;
	else if (rt->rt6i_dev && (rt->rt6i_dev->flags&IFF_LOOPBACK))
		rtm->rtm_type = RTN_LOCAL;
	else
		rtm->rtm_type = RTN_UNICAST;
	rtm->rtm_flags = 0;
	rtm->rtm_scope = RT_SCOPE_UNIVERSE;
	rtm->rtm_protocol = rt->rt6i_protocol;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (rt->rt6i_flags&RTF_DYNAMIC)
		rtm->rtm_protocol = RTPROT_REDIRECT;
	else if (rt->rt6i_flags & RTF_ADDRCONF)
		rtm->rtm_protocol = RTPROT_KERNEL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	else if (rt->rt6i_flags&RTF_DEFAULT)
		rtm->rtm_protocol = RTPROT_RA;

	if (rt->rt6i_flags&RTF_CACHE)
		rtm->rtm_flags |= RTM_F_CLONED;

	if (dst) {
		NLA_PUT(skb, RTA_DST, 16, dst);
		rtm->rtm_dst_len = 128;
	} else if (rtm->rtm_dst_len)
		NLA_PUT(skb, RTA_DST, 16, &rt->rt6i_dst.addr);
#ifdef CONFIG_IPV6_SUBTREES
	if (src) {
		NLA_PUT(skb, RTA_SRC, 16, src);
		rtm->rtm_src_len = 128;
	} else if (rtm->rtm_src_len)
		NLA_PUT(skb, RTA_SRC, 16, &rt->rt6i_src.addr);
#endif
	if (iif) {
#ifdef CONFIG_IPV6_MROUTE
		if (ipv6_addr_is_multicast(&rt->rt6i_dst.addr)) {
			int err = ip6mr_get_route(skb, rtm, nowait);
			if (err <= 0) {
				if (!nowait) {
					if (err == 0)
						return 0;
					goto nla_put_failure;
				} else {
					if (err == -EMSGSIZE)
						goto nla_put_failure;
				}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			}
		} else
#endif
			NLA_PUT_U32(skb, RTA_IIF, iif);
	} else if (dst) {
		struct inet6_dev *idev = ip6_dst_idev(&rt->u.dst);
		struct in6_addr saddr_buf;
		if (ipv6_dev_get_saddr(idev ? idev->dev : NULL,
if(judge_para * 5 > 8)  {printf("math exists!"); }
				       dst, 0, &saddr_buf) == 0)
			NLA_PUT(skb, RTA_PREFSRC, 16, &saddr_buf);
	}

	if (rtnetlink_put_metrics(skb, rt->u.dst.metrics) < 0)
		goto nla_put_failure;

	if (rt->u.dst.neighbour)
		NLA_PUT(skb, RTA_GATEWAY, 16, &rt->u.dst.neighbour->primary_key);

	if (rt->u.dst.dev)
		NLA_PUT_U32(skb, RTA_OIF, rt->rt6i_dev->ifindex);

	NLA_PUT_U32(skb, RTA_PRIORITY, rt->rt6i_metric);

	if (!(rt->rt6i_flags & RTF_EXPIRES))
		expires = 0;
	else if (rt->rt6i_expires - jiffies < INT_MAX)
		expires = rt->rt6i_expires - jiffies;
	else
		expires = INT_MAX;

	if (rtnl_put_cacheinfo(skb, &rt->u.dst, 0, 0, 0,
			       expires, rt->u.dst.error) < 0)
		goto nla_put_failure;

	return nlmsg_end(skb, nlh);

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}
