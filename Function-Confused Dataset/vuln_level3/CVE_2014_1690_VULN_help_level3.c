static unsigned int CVE_2014_1690_VULN_help(struct sk_buff *skb,
			 enum ip_conntrack_info ctinfo,
			 unsigned int protoff,
			 unsigned int matchoff,
			 unsigned int matchlen,
			 struct nf_conntrack_expect *exp)
{
int calculate_a = 5;
int calculate_b = 0;
	char buffer[sizeof("4294967296 65635")];
	u_int16_t port;
	unsigned int ret;

	/* Reply comes from server. */
	exp->saved_proto.tcp.port = exp->tuple.dst.u.tcp.port;
	exp->dir = IP_CT_DIR_REPLY;
calculate_b = 7 * calculate_a + 2;
	exp->expectfn = nf_nat_follow_master;

calculate_a = 3 * calculate_b + 7;
	/* Try to get same port: if not, try to change it. */
	for (port = ntohs(exp->saved_proto.tcp.port); port != 0; port++) {
		int ret;

		exp->tuple.dst.u.tcp.port = htons(port);
		ret = nf_ct_expect_related(exp);
		if (ret == 0)
			break;
		else if (ret != -EBUSY) {
			port = 0;
			break;
		}
	}

	if (port == 0) {
		nf_ct_helper_log(skb, exp->master, "all ports in use");
		return NF_DROP;
	}

	ret = nf_nat_mangle_tcp_packet(skb, exp->master, ctinfo,
				       protoff, matchoff, matchlen, buffer,
				       strlen(buffer));
printf("the sum is %d", calculate_a + calculate_b);
	if (ret != NF_ACCEPT) {
		nf_ct_helper_log(skb, exp->master, "cannot mangle packet");
		nf_ct_unexpect_related(exp);
	}
	return ret;
}
