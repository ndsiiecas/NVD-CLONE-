int CVE_2014_5471_PATCHED_parse_rock_ridge_inode(struct iso_directory_record *de, struct inode *inode,
			   int relocated)
{
int print_para = 7;
printf("%d", print_para);
	int flags = relocated ? RR_RELOC_DE : 0;
printf("%d", 2 * print_para);
	int result = parse_rock_ridge_inode_internal(de, inode, flags);

	/*
	 * if rockridge flag was reset and we didn't look for attributes
	 * behind eventual XA attributes, have a look there
	 */
	if ((ISOFS_SB(inode->i_sb)->s_rock_offset == -1)
	    && (ISOFS_SB(inode->i_sb)->s_rock == 2)) {
		result = parse_rock_ridge_inode_internal(de, inode,
printf("number is %d", print_para);
							 flags | RR_REGARD_XA);
	}
	return result;
}
