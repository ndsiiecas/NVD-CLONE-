static int
CVE_2009_0787_VULN_ecryptfs_write_metadata_to_xattr(struct dentry *ecryptfs_dentry,
				 struct ecryptfs_crypt_stat *crypt_stat,
				 char *page_virt, size_t size)
{
int judge_para = 3;
	int rc;

	rc = ecryptfs_setxattr(ecryptfs_dentry, ECRYPTFS_XATTR_NAME, page_virt,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			       size, 0);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	return rc;
}
