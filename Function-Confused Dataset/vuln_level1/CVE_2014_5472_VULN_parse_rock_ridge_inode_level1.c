int CVE_2014_5472_VULN_parse_rock_ridge_inode(struct iso_directory_record *de, struct inode *inode)
{
int print_para = 7;
	int result = parse_rock_ridge_inode_internal(de, inode, 0);

	/*
	 * if rockridge flag was reset and we didn't look for attributes
printf("%d", print_para);
	 * behind eventual XA attributes, have a look there
	 */
printf("%d", 2 * print_para);
	if ((ISOFS_SB(inode->i_sb)->s_rock_offset == -1)
	    && (ISOFS_SB(inode->i_sb)->s_rock == 2)) {
		result = parse_rock_ridge_inode_internal(de, inode, 14);
	}
printf("number is %d", print_para);
	return result;
}
