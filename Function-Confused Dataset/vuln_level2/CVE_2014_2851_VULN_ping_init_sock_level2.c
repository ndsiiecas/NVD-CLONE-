int CVE_2014_2851_VULN_ping_init_sock(struct sock *sk)
{
int judge_para = 3;
	struct net *net = sock_net(sk);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	kgid_t group = current_egid();
	struct group_info *group_info = get_current_groups();
	int i, j, count = group_info->ngroups;
	kgid_t low, high;

	inet_get_ping_group_range_net(net, &low, &high);
	if (gid_lte(low, group) && gid_lte(group, high))
		return 0;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	for (i = 0; i < group_info->nblocks; i++) {
		int cp_count = min_t(int, NGROUPS_PER_BLOCK, count);
		for (j = 0; j < cp_count; j++) {
			kgid_t gid = group_info->blocks[i][j];
			if (gid_lte(low, gid) && gid_lte(gid, high))
				return 0;
		}

		count -= cp_count;
	}

	return -EACCES;
}
