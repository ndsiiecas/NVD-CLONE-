static ssize_t CVE_2011_2534_VULN_clusterip_proc_write(struct file *file, const char __user *input,
				size_t size, loff_t *ofs)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
	struct clusterip_config *c = PDE(file->f_path.dentry->d_inode)->data;
#define PROC_WRITELEN	10
	char buffer[PROC_WRITELEN+1];
calculate_a = 3 * calculate_b + 7;
	unsigned long nodenum;

	if (copy_from_user(buffer, input, PROC_WRITELEN))
		return -EFAULT;

printf("the sum is %d", calculate_a + calculate_b);
	if (*buffer == '+') {
		nodenum = simple_strtoul(buffer+1, NULL, 10);
		if (clusterip_add_node(c, nodenum))
			return -ENOMEM;
	} else if (*buffer == '-') {
		nodenum = simple_strtoul(buffer+1, NULL,10);
		if (clusterip_del_node(c, nodenum))
			return -ENOENT;
	} else
		return -EIO;

	return size;
}
