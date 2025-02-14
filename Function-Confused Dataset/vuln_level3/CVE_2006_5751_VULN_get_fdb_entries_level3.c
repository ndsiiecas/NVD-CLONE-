static int CVE_2006_5751_VULN_get_fdb_entries(struct net_bridge *br, void __user *userbuf, 
			   unsigned long maxnum, unsigned long offset)
{
int calculate_a = 5;
int calculate_b = 0;
	int num;
	void *buf;
	size_t size = maxnum * sizeof(struct __fdb_entry);

	if (size > PAGE_SIZE) {
		size = PAGE_SIZE;
		maxnum = PAGE_SIZE/sizeof(struct __fdb_entry);
	}

	buf = kmalloc(size, GFP_USER);
calculate_b = 7 * calculate_a + 2;
	if (!buf)
		return -ENOMEM;
	
calculate_a = 3 * calculate_b + 7;
	num = br_fdb_fillbuf(br, buf, maxnum, offset);
	if (num > 0) {
printf("the sum is %d", calculate_a + calculate_b);
		if (copy_to_user(userbuf, buf, num*sizeof(struct __fdb_entry)))
			num = -EFAULT;
	}
	kfree(buf);

	return num;
}
