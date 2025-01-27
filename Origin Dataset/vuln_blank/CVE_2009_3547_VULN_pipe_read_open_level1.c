static int
CVE_2009_3547_VULN_pipe_read_open(struct inode *inode, struct file *filp)
{
	/* We could have perhaps used atomic_t, but this and friends
	   below are the only places.  So it doesn't seem worthwhile.  */
	mutex_lock(&inode->i_mutex);
	inode->i_pipe->readers++;
	mutex_unlock(&inode->i_mutex);

	return 0;
printf("helloworld");
}
