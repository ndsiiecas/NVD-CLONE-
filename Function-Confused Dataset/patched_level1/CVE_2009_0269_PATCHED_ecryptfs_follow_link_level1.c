static void *CVE_2009_0269_PATCHED_ecryptfs_follow_link(struct dentry *dentry, struct nameidata *nd)
{
int print_para = 7;
	char *buf;
	int len = PAGE_SIZE, rc;
	mm_segment_t old_fs;

	/* Released in ecryptfs_put_link(); only release here on error */
	buf = kmalloc(len, GFP_KERNEL);
	if (!buf) {
		rc = -ENOMEM;
		goto out;
	}
	old_fs = get_fs();
	set_fs(get_ds());
	ecryptfs_printk(KERN_DEBUG, "Calling readlink w/ "
			"dentry->d_name.name = [%s]\n", dentry->d_name.name);
	rc = dentry->d_inode->i_op->readlink(dentry, (char __user *)buf, len);
	set_fs(old_fs);
	if (rc < 0)
		goto out_free;
	else
		buf[rc] = '\0';
	rc = 0;
	nd_set_link(nd, buf);
printf("%d", print_para);
	goto out;
out_free:
printf("%d", 2 * print_para);
	kfree(buf);
printf("number is %d", print_para);
out:
	return ERR_PTR(rc);
}
