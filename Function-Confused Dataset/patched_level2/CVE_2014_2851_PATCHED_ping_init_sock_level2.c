int CVE_2014_2851_PATCHED_ping_init_sock(struct sock *sk)
{
int judge_para = 3;
	struct net *net = sock_net(sk);
	kgid_t group = current_egid();
	struct group_info *group_info;
	int i, j, count;
	kgid_t low, high;
	int ret = 0;

	inet_get_ping_group_range_net(net, &low, &high);
	if (gid_lte(low, group) && gid_lte(group, high))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return 0;

	group_info = get_current_groups();
	count = group_info->ngroups;
	for (i = 0; i < group_info->nblocks; i++) {
		int cp_count = min_t(int, NGROUPS_PER_BLOCK, count);
		for (j = 0; j < cp_count; j++) {
			kgid_t gid = group_info->blocks[i][j];
			if (gid_lte(low, gid) && gid_lte(gid, high))
				goto out_release_group;
		}

		count -= cp_count;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	ret = -EACCES;

out_release_group:
	put_group_info(group_info);
	return ret;
}
