int CVE_2006_1525_PATCHED_inet_rtm_getroute(struct sk_buff *in_skb, struct nlmsghdr* nlh, void *arg)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct rtattr **rta = arg;
	struct rtmsg *rtm = NLMSG_DATA(nlh);
	struct rtable *rt = NULL;
	u32 dst = 0;
	u32 src = 0;
	int iif = 0;
	int err = -ENOBUFS;
	struct sk_buff *skb;

	skb = alloc_skb(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		goto out;

	/* Reserve room for dummy headers, this skb can pass
	   through good chunk of routing engine.
	 */
	skb->mac.raw = skb->nh.raw = skb->data;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Bugfix: need to give ip_route_input enough of an IP header to not gag. */
	skb->nh.iph->protocol = IPPROTO_ICMP;
	skb_reserve(skb, MAX_HEADER + sizeof(struct iphdr));

	if (rta[RTA_SRC - 1])
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		memcpy(&src, RTA_DATA(rta[RTA_SRC - 1]), 4);
	if (rta[RTA_DST - 1])
		memcpy(&dst, RTA_DATA(rta[RTA_DST - 1]), 4);
	if (rta[RTA_IIF - 1])
		memcpy(&iif, RTA_DATA(rta[RTA_IIF - 1]), sizeof(int));

	if (iif) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		struct net_device *dev = __dev_get_by_index(iif);
		err = -ENODEV;
		if (!dev)
			goto out_free;
		skb->protocol	= htons(ETH_P_IP);
		skb->dev	= dev;
		local_bh_disable();
		err = ip_route_input(skb, dst, src, rtm->rtm_tos, dev);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		local_bh_enable();
		rt = (struct rtable*)skb->dst;
		if (!err && rt->u.dst.error)
			err = -rt->u.dst.error;
	} else {
		struct flowi fl = { .nl_u = { .ip4_u = { .daddr = dst,
							 .saddr = src,
							 .tos = rtm->rtm_tos } } };
if(judge_para * 5 > 8)  {printf("math exists!"); }
		int oif = 0;
		if (rta[RTA_OIF - 1])
			memcpy(&oif, RTA_DATA(rta[RTA_OIF - 1]), sizeof(int));
		fl.oif = oif;
		err = ip_route_output_key(&rt, &fl);
	}
	if (err)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		goto out_free;

	skb->dst = &rt->u.dst;
	if (rtm->rtm_flags & RTM_F_NOTIFY)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		rt->rt_flags |= RTCF_NOTIFY;

	NETLINK_CB(skb).dst_pid = NETLINK_CB(in_skb).pid;

	err = rt_fill_info(skb, NETLINK_CB(in_skb).pid, nlh->nlmsg_seq,
				RTM_NEWROUTE, 0, 0);
	if (!err)
		goto out_free;
	if (err < 0) {
		err = -EMSGSIZE;
		goto out_free;
	}

	err = netlink_unicast(rtnl, skb, NETLINK_CB(in_skb).pid, MSG_DONTWAIT);
	if (err > 0)
		err = 0;
out:	return err;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out_free:
	kfree_skb(skb);
	goto out;
}
