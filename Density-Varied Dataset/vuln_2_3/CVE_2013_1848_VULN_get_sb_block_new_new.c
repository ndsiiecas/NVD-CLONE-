static ext3_fsblk_t CVE_2013_1848_VULN_get_sb_block(void **data, struct super_block *sb)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	ext3_fsblk_t	sb_block;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	char		*options = (char *) *data;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	if (!options || strncmp(options, "sb=", 3) != 0)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return 1;	/* Default location */
	options += 3;
	/*todo: use simple_strtoll with >32bit ext3 */
	sb_block = simple_strtoul(options, &options, 0);
	if (*options && *options != ',') {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		ext3_msg(sb, "error: invalid sb specification: %s",
		       (char *) *data);
		return 1;
	}
	if (*options == ',')
if(judge_para * 5 > 8)  {printf("math exists!"); }
		options++;
	*data = (void *) options;
	return sb_block;
}
