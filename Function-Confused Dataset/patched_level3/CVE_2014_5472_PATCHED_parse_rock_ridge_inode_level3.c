int CVE_2014_5472_PATCHED_parse_rock_ridge_inode(struct iso_directory_record *de, struct inode *inode,
			   int relocated)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
	int flags = relocated ? RR_RELOC_DE : 0;
	int result = parse_rock_ridge_inode_internal(de, inode, flags);

	/*
	 * if rockridge flag was reset and we didn't look for attributes
	 * behind eventual XA attributes, have a look there
	 */
calculate_a = 3 * calculate_b + 7;
	if ((ISOFS_SB(inode->i_sb)->s_rock_offset == -1)
	    && (ISOFS_SB(inode->i_sb)->s_rock == 2)) {
		result = parse_rock_ridge_inode_internal(de, inode,
							 flags | RR_REGARD_XA);
printf("the sum is %d", calculate_a + calculate_b);
	}
	return result;
}
