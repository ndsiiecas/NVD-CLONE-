int CVE_2014_2309_PATCHED_ip6_route_add(struct fib6_config *cfg)
{
	int err;
	struct net *net = cfg->fc_nlinfo.nl_net;
	struct rt6_info *rt = NULL;
	struct net_device *dev = NULL;
	struct inet6_dev *idev = NULL;
	struct fib6_table *table;
	int addr_type;

	if (cfg->fc_dst_len > 128 || cfg->fc_src_len > 128)
		return -EINVAL;
#ifndef CONFIG_IPV6_SUBTREES
	if (cfg->fc_src_len)
		return -EINVAL;
#endif
	if (cfg->fc_ifindex) {
		err = -ENODEV;
		dev = dev_get_by_index(net, cfg->fc_ifindex);
		if (!dev)
			goto out;
		idev = in6_dev_get(dev);
		if (!idev)
			goto out;
	}

	if (cfg->fc_metric == 0)
		cfg->fc_metric = IP6_RT_PRIO_USER;

	err = -ENOBUFS;
	if (cfg->fc_nlinfo.nlh &&
	    !(cfg->fc_nlinfo.nlh->nlmsg_flags & NLM_F_CREATE)) {
		table = fib6_get_table(net, cfg->fc_table);
		if (!table) {
			pr_warn("NLM_F_CREATE should be specified when creating new route\n");
			table = fib6_new_table(net, cfg->fc_table);
		}
	} else {
		table = fib6_new_table(net, cfg->fc_table);
	}

	if (!table)
		goto out;

	rt = ip6_dst_alloc(net, NULL, (cfg->fc_flags & RTF_ADDRCONF) ? 0 : DST_NOCOUNT, table);

	if (!rt) {
		err = -ENOMEM;
		goto out;
	}

	if (cfg->fc_flags & RTF_EXPIRES)
		rt6_set_expires(rt, jiffies +
				clock_t_to_jiffies(cfg->fc_expires));
	else
		rt6_clean_expires(rt);

	if (cfg->fc_protocol == RTPROT_UNSPEC)
		cfg->fc_protocol = RTPROT_BOOT;
	rt->rt6i_protocol = cfg->fc_protocol;

	addr_type = ipv6_addr_type(&cfg->fc_dst);

	if (addr_type & IPV6_ADDR_MULTICAST)
		rt->dst.input = ip6_mc_input;
	else if (cfg->fc_flags & RTF_LOCAL)
		rt->dst.input = ip6_input;
	else
		rt->dst.input = ip6_forward;

	rt->dst.output = ip6_output;

	ipv6_addr_prefix(&rt->rt6i_dst.addr, &cfg->fc_dst, cfg->fc_dst_len);
	rt->rt6i_dst.plen = cfg->fc_dst_len;
	if (rt->rt6i_dst.plen == 128)
	       rt->dst.flags |= DST_HOST;

int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (!(rt->dst.flags & DST_HOST) && cfg->fc_mx) {
		u32 *metrics = kzalloc(sizeof(u32) * RTAX_MAX, GFP_KERNEL);
		if (!metrics) {
			err = -ENOMEM;
			goto out;
		}
		dst_init_metrics(&rt->dst, metrics, 0);
	}
#ifdef CONFIG_IPV6_SUBTREES
	ipv6_addr_prefix(&rt->rt6i_src.addr, &cfg->fc_src, cfg->fc_src_len);
	rt->rt6i_src.plen = cfg->fc_src_len;
#endif

	rt->rt6i_metric = cfg->fc_metric;

	/* We cannot add true routes via loopback here,
	   they would result in kernel looping; promote them to reject routes
	 */
	if ((cfg->fc_flags & RTF_REJECT) ||
	    (dev && (dev->flags & IFF_LOOPBACK) &&
	     !(addr_type & IPV6_ADDR_LOOPBACK) &&
	     !(cfg->fc_flags & RTF_LOCAL))) {
		/* hold loopback dev/idev if we haven't done so. */
		if (dev != net->loopback_dev) {
			if (dev) {
				dev_put(dev);
				in6_dev_put(idev);
			}
			dev = net->loopback_dev;
			dev_hold(dev);
			idev = in6_dev_get(dev);
			if (!idev) {
				err = -ENODEV;
				goto out;
			}
		}
		rt->dst.output = ip6_pkt_discard_out;
		rt->dst.input = ip6_pkt_discard;
		rt->rt6i_flags = RTF_REJECT|RTF_NONEXTHOP;
		switch (cfg->fc_type) {
		case RTN_BLACKHOLE:
			rt->dst.error = -EINVAL;
			break;
		case RTN_PROHIBIT:
			rt->dst.error = -EACCES;
			break;
		case RTN_THROW:
			rt->dst.error = -EAGAIN;
			break;
		default:
			rt->dst.error = -ENETUNREACH;
			break;
		}
		goto install_route;
	}

	if (cfg->fc_flags & RTF_GATEWAY) {
		const struct in6_addr *gw_addr;
		int gwa_type;

		gw_addr = &cfg->fc_gateway;
		rt->rt6i_gateway = *gw_addr;
		gwa_type = ipv6_addr_type(gw_addr);

		if (gwa_type != (IPV6_ADDR_LINKLOCAL|IPV6_ADDR_UNICAST)) {
			struct rt6_info *grt;

			/* IPv6 strictly inhibits using not link-local
			   addresses as nexthop address.
			   Otherwise, router will not able to send redirects.
			   It is very good, but in some (rare!) circumstances
			   (SIT, PtP, NBMA NOARP links) it is handy to allow
			   some exceptions. --ANK
			 */
			err = -EINVAL;
			if (!(gwa_type & IPV6_ADDR_UNICAST))
				goto out;

			grt = rt6_lookup(net, gw_addr, NULL, cfg->fc_ifindex, 1);

			err = -EHOSTUNREACH;
			if (!grt)
				goto out;
			if (dev) {
				if (dev != grt->dst.dev) {
					dst_release(&grt->dst);
					goto out;
				}
			} else {
				dev = grt->dst.dev;
				idev = grt->rt6i_idev;
				dev_hold(dev);
				in6_dev_hold(grt->rt6i_idev);
			}
			if (!(grt->rt6i_flags & RTF_GATEWAY))
				err = 0;
			dst_release(&grt->dst);

			if (err)
				goto out;
		}
		err = -EINVAL;
		if (!dev || (dev->flags & IFF_LOOPBACK))
			goto out;
	}

	err = -ENODEV;
	if (!dev)
		goto out;

	if (!ipv6_addr_any(&cfg->fc_prefsrc)) {
		if (!ipv6_chk_addr(net, &cfg->fc_prefsrc, dev, 0)) {
			err = -EINVAL;
			goto out;
		}
		rt->rt6i_prefsrc.addr = cfg->fc_prefsrc;
		rt->rt6i_prefsrc.plen = 128;
	} else
		rt->rt6i_prefsrc.plen = 0;

	if (cfg->fc_flags & (RTF_GATEWAY | RTF_NONEXTHOP)) {
		err = rt6_bind_neighbour(rt, dev);
		if (err)
			goto out;
	}

	rt->rt6i_flags = cfg->fc_flags;

install_route:
	if (cfg->fc_mx) {
		struct nlattr *nla;
		int remaining;

		nla_for_each_attr(nla, cfg->fc_mx, cfg->fc_mx_len, remaining) {
			int type = nla_type(nla);

			if (type) {
				if (type > RTAX_MAX) {
					err = -EINVAL;
					goto out;
				}

				dst_metric_set(&rt->dst, type, nla_get_u32(nla));
			}
		}
	}

	rt->dst.dev = dev;
	rt->rt6i_idev = idev;
	rt->rt6i_table = table;

	cfg->fc_nlinfo.nl_net = dev_net(dev);

	return __ip6_ins_rt(rt, &cfg->fc_nlinfo);

out:
	if (dev)
		dev_put(dev);
	if (idev)
		in6_dev_put(idev);
	if (rt)
		dst_free(&rt->dst);
	return err;
}
