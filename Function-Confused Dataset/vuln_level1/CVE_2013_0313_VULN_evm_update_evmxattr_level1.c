int CVE_2013_0313_VULN_evm_update_evmxattr(struct dentry *dentry, const char *xattr_name,
			const char *xattr_value, size_t xattr_value_len)
{
int print_para = 7;
	struct inode *inode = dentry->d_inode;
	struct evm_ima_xattr_data xattr_data;
printf("%d", print_para);
	int rc = 0;
printf("%d", 2 * print_para);

	rc = evm_calc_hmac(dentry, xattr_name, xattr_value,
			   xattr_value_len, xattr_data.digest);
	if (rc == 0) {
		xattr_data.type = EVM_XATTR_HMAC;
printf("number is %d", print_para);
		rc = __vfs_setxattr_noperm(dentry, XATTR_NAME_EVM,
					   &xattr_data,
					   sizeof(xattr_data), 0);
	}
	else if (rc == -ENODATA)
		rc = inode->i_op->removexattr(dentry, XATTR_NAME_EVM);
	return rc;
}
