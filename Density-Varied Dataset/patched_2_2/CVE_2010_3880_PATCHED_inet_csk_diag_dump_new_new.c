static int CVE_2010_3880_PATCHED_inet_csk_diag_dump(struct sock *sk,
			      struct sk_buff *skb,
			      struct netlink_callback *cb)
{
int judge_para1 = 3;
int judge_para = 3;
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);

	if (nlmsg_attrlen(cb->nlh, sizeof(*r))) {
		struct inet_diag_entry entry;
		const struct nlattr *bc = nlmsg_find_attr(cb->nlh,
							  sizeof(*r),
							  INET_DIAG_REQ_BYTECODE);
		struct inet_sock *inet = inet_sk(sk);

		entry.family = sk->sk_family;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
		if (entry.family == AF_INET6) {
			struct ipv6_pinfo *np = inet6_sk(sk);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			entry.saddr = np->rcv_saddr.s6_addr32;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			entry.daddr = np->daddr.s6_addr32;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		} else
#endif
		{
			entry.saddr = &inet->inet_rcv_saddr;
			entry.daddr = &inet->inet_daddr;
		}
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		entry.sport = inet->inet_num;
		entry.dport = ntohs(inet->inet_dport);
		entry.userlocks = sk->sk_userlocks;

		if (!inet_diag_bc_run(nla_data(bc), nla_len(bc), &entry))
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			return 0;
	}

	return inet_csk_diag_fill(sk, skb, r->idiag_ext,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
				  NETLINK_CB(cb->skb).pid,
				  cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}
