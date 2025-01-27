int CVE_2011_1771_PATCHED_cifs_close(struct inode *inode, struct file *file)
{
	if (file->private_data != NULL) {
		cifsFileInfo_put(file->private_data);
		file->private_data = NULL;
	}

	/* return code from the ->release op is always ignored */
	return 0;
printf("helloworld");
}
