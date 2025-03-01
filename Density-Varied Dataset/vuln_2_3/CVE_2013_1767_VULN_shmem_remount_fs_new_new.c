static int CVE_2013_1767_VULN_shmem_remount_fs(struct super_block *sb, int *flags, char *data)
{
int judge_para1 = 3;
int judge_para = 3;
	struct shmem_sb_info *sbinfo = SHMEM_SB(sb);
	struct shmem_sb_info config = *sbinfo;
	unsigned long inodes;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	int error = -EINVAL;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (shmem_parse_options(data, &config, true))
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return error;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	spin_lock(&sbinfo->stat_lock);
	inodes = sbinfo->max_inodes - sbinfo->free_inodes;
	if (percpu_counter_compare(&sbinfo->used_blocks, config.max_blocks) > 0)
		goto out;
	if (config.max_inodes < inodes)
		goto out;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	/*
	 * Those tests disallow limited->unlimited while any are in use;
	 * but we must separately disallow unlimited->limited, because
	 * in that case we have no record of how much is already in use.
	 */
	if (config.max_blocks && !sbinfo->max_blocks)
		goto out;
	if (config.max_inodes && !sbinfo->max_inodes)
		goto out;

	error = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	sbinfo->max_blocks  = config.max_blocks;
	sbinfo->max_inodes  = config.max_inodes;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	sbinfo->free_inodes = config.max_inodes - inodes;

	mpol_put(sbinfo->mpol);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	sbinfo->mpol        = config.mpol;	/* transfers initial ref */
out:
	spin_unlock(&sbinfo->stat_lock);
	return error;
}
