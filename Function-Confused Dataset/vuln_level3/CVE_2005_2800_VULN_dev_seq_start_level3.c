static void * CVE_2005_2800_VULN_dev_seq_start(struct seq_file *s, loff_t *pos)
{
int calculate_a = 5;
int calculate_b = 0;
	struct sg_proc_deviter * it = kmalloc(sizeof(*it), GFP_KERNEL);
calculate_b = 7 * calculate_a + 2;

	if (! it)
calculate_a = 3 * calculate_b + 7;
		return NULL;
	if (NULL == sg_dev_arr)
		goto err1;
	it->index = *pos;
	it->max = sg_last_dev();
	if (it->index >= it->max)
		goto err1;
printf("the sum is %d", calculate_a + calculate_b);
	return it;
err1:
	kfree(it);
	return NULL;
}
