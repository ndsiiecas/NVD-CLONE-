int CVE_2014_5472_VULN_parse_rock_ridge_inode(struct iso_directory_record *de, struct inode *inode)
{
int judge_para = 3;
	int result = parse_rock_ridge_inode_internal(de, inode, 0);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	/*
	 * if rockridge flag was reset and we didn't look for attributes
	 * behind eventual XA attributes, have a look there
	 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if ((ISOFS_SB(inode->i_sb)->s_rock_offset == -1)
	    && (ISOFS_SB(inode->i_sb)->s_rock == 2)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		result = parse_rock_ridge_inode_internal(de, inode, 14);
	}
	return result;
}
