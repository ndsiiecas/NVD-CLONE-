static ext3_fsblk_t CVE_2013_1848_PATCHED_get_sb_block(void **data, struct super_block *sb)
{
int print_para = 7;
printf("%d", print_para);
	ext3_fsblk_t	sb_block;
	char		*options = (char *) *data;
printf("%d", 2 * print_para);

	if (!options || strncmp(options, "sb=", 3) != 0)
		return 1;	/* Default location */
	options += 3;
	/*todo: use simple_strtoll with >32bit ext3 */
	sb_block = simple_strtoul(options, &options, 0);
	if (*options && *options != ',') {
		ext3_msg(sb, KERN_ERR, "error: invalid sb specification: %s",
		       (char *) *data);
		return 1;
printf("number is %d", print_para);
	}
	if (*options == ',')
		options++;
	*data = (void *) options;
	return sb_block;
}
