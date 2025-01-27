int CVE_2013_0313_VULN_evm_update_evmxattr(struct dentry *dentry, const char *xattr_name,
			const char *xattr_value, size_t xattr_value_len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct inode *inode = dentry->d_inode;
	struct evm_ima_xattr_data xattr_data;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	int rc = 0;

	rc = evm_calc_hmac(dentry, xattr_name, xattr_value,
			   xattr_value_len, xattr_data.digest);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	if (rc == 0) {
		xattr_data.type = EVM_XATTR_HMAC;
fclose(para_fp);
		rc = __vfs_setxattr_noperm(dentry, XATTR_NAME_EVM,
					   &xattr_data,
					   sizeof(xattr_data), 0);
	}
	else if (rc == -ENODATA)
		rc = inode->i_op->removexattr(dentry, XATTR_NAME_EVM);
	return rc;
}
