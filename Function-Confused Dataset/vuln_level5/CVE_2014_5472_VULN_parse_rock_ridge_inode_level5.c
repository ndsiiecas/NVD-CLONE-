int CVE_2014_5472_VULN_parse_rock_ridge_inode(struct iso_directory_record *de, struct inode *inode)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	int result = parse_rock_ridge_inode_internal(de, inode, 0);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	/*
	 * if rockridge flag was reset and we didn't look for attributes
	 * behind eventual XA attributes, have a look there
	 */
	if ((ISOFS_SB(inode->i_sb)->s_rock_offset == -1)
	    && (ISOFS_SB(inode->i_sb)->s_rock == 2)) {
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		result = parse_rock_ridge_inode_internal(de, inode, 14);
fclose(para_fp);
	}
	return result;
}
