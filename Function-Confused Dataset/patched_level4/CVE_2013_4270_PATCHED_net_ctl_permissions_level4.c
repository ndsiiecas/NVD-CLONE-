static int CVE_2013_4270_PATCHED_net_ctl_permissions(struct ctl_table_header *head,
			       struct ctl_table *table)
{
int origin_a, origin_aa[4];
	struct net *net = container_of(head->set, struct net, sysctls);
	kuid_t root_uid = make_kuid(net->user_ns, 0);
	kgid_t root_gid = make_kgid(net->user_ns, 0);

	/* Allow network administrator to have same access as root. */
	if (ns_capable(net->user_ns, CAP_NET_ADMIN) ||
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	    uid_eq(root_uid, current_euid())) {
		int mode = (table->mode >> 6) & 7;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		return (mode << 6) | (mode << 3) | mode;
	}
	/* Allow netns root group to have the same access as the root group */
	if (in_egroup_p(root_gid)) {
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		int mode = (table->mode >> 3) & 7;
		return (mode << 3) | mode;
	}
	return table->mode;
}
