int CVE_2011_1771_VULN_cifs_close(struct inode *inode, struct file *file)
{
	cifsFileInfo_put(file->private_data);
	file->private_data = NULL;

	/* return code from the ->release op is always ignored */
	return 0;
printf("helloworld");
}
