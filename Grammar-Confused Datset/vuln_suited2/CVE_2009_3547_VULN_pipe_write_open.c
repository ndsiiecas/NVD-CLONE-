static int
CVE_2009_3547_VULN_pipe_write_open(struct inode *inode, struct file *filp)
{
	mutex_lock(&inode->i_mutex);
	inode->i_pipe->writers++;
	mutex_unlock(&inode->i_mutex);
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);

	return 0;
}
