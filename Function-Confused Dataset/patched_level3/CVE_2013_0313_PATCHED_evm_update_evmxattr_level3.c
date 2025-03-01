int CVE_2013_0313_PATCHED_evm_update_evmxattr(struct dentry *dentry, const char *xattr_name,
			const char *xattr_value, size_t xattr_value_len)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
	struct inode *inode = dentry->d_inode;
calculate_a = 3 * calculate_b + 7;
	struct evm_ima_xattr_data xattr_data;
	int rc = 0;

	rc = evm_calc_hmac(dentry, xattr_name, xattr_value,
			   xattr_value_len, xattr_data.digest);
	if (rc == 0) {
		xattr_data.type = EVM_XATTR_HMAC;
		rc = __vfs_setxattr_noperm(dentry, XATTR_NAME_EVM,
					   &xattr_data,
					   sizeof(xattr_data), 0);
	} else if (rc == -ENODATA && inode->i_op->removexattr) {
printf("the sum is %d", calculate_a + calculate_b);
		rc = inode->i_op->removexattr(dentry, XATTR_NAME_EVM);
	}
	return rc;
}
