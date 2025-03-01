static int CVE_2013_1767_VULN_shmem_remount_fs(struct super_block *sb, int *flags, char *data)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct shmem_sb_info *sbinfo = SHMEM_SB(sb);
	struct shmem_sb_info config = *sbinfo;
	unsigned long inodes;
	int error = -EINVAL;

	if (shmem_parse_options(data, &config, true))
		return error;

	spin_lock(&sbinfo->stat_lock);
	inodes = sbinfo->max_inodes - sbinfo->free_inodes;
	if (percpu_counter_compare(&sbinfo->used_blocks, config.max_blocks) > 0)
		goto out;
	if (config.max_inodes < inodes)
		goto out;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
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
	sbinfo->max_blocks  = config.max_blocks;
	sbinfo->max_inodes  = config.max_inodes;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	sbinfo->free_inodes = config.max_inodes - inodes;

	mpol_put(sbinfo->mpol);
	sbinfo->mpol        = config.mpol;	/* transfers initial ref */
out:
	spin_unlock(&sbinfo->stat_lock);
fclose(para_fp);
	return error;
}
