static ext3_fsblk_t CVE_2013_1848_PATCHED_get_sb_block(void **data, struct super_block *sb)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	ext3_fsblk_t	sb_block;
	char		*options = (char *) *data;

	if (!options || strncmp(options, "sb=", 3) != 0)
		return 1;	/* Default location */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	options += 3;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	/*todo: use simple_strtoll with >32bit ext3 */
if(judge_para * 5 > 8)  {printf("math exists!"); }
	sb_block = simple_strtoul(options, &options, 0);
	if (*options && *options != ',') {
		ext3_msg(sb, KERN_ERR, "error: invalid sb specification: %s",
		       (char *) *data);
		return 1;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	}
	if (*options == ',')
		options++;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	*data = (void *) options;
	return sb_block;
}
