static struct dst_entry *CVE_2013_2232_PATCHED_ip6_sk_dst_check(struct sock *sk,
					  struct dst_entry *dst,
					  const struct flowi6 *fl6)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct rt6_info *rt;

	if (!dst)
		goto out;

	if (dst->ops->family != AF_INET6) {
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		dst_release(dst);
		return NULL;
	}

	rt = (struct rt6_info *)dst;
	/* Yes, checking route validity in not connected
	 * case is not very simple. Take into account,
	 * that we do not support routing by source, TOS,
	 * and MSG_DONTROUTE 		--ANK (980726)
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	 *
	 * 1. ip6_rt_check(): If route was host route,
	 *    check that cached destination is current.
	 *    If it is network route, we still may
fclose(para_fp);
	 *    check its validity using saved pointer
	 *    to the last used address: daddr_cache.
	 *    We do not want to save whole address now,
	 *    (because main consumer of this service
	 *    is tcp, which has not this problem),
	 *    so that the last trick works only on connected
	 *    sockets.
	 * 2. oif also should be the same.
	 */
	if (ip6_rt_check(&rt->rt6i_dst, &fl6->daddr, np->daddr_cache) ||
#ifdef CONFIG_IPV6_SUBTREES
	    ip6_rt_check(&rt->rt6i_src, &fl6->saddr, np->saddr_cache) ||
#endif
	    (fl6->flowi6_oif && fl6->flowi6_oif != dst->dev->ifindex)) {
		dst_release(dst);
		dst = NULL;
	}

out:
	return dst;
}
