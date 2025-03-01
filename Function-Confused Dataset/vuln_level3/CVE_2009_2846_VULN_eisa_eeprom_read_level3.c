static ssize_t CVE_2009_2846_VULN_eisa_eeprom_read(struct file * file,
			      char __user *buf, size_t count, loff_t *ppos )
{
int calculate_a = 5;
int calculate_b = 0;
	unsigned char *tmp;
	ssize_t ret;
	int i;
	
	if (*ppos >= HPEE_MAX_LENGTH)
		return 0;
	
	count = *ppos + count < HPEE_MAX_LENGTH ? count : HPEE_MAX_LENGTH - *ppos;
	tmp = kmalloc(count, GFP_KERNEL);
	if (tmp) {
		for (i = 0; i < count; i++)
			tmp[i] = readb(eisa_eeprom_addr+(*ppos)++);
calculate_b = 7 * calculate_a + 2;

		if (copy_to_user (buf, tmp, count))
			ret = -EFAULT;
		else
			ret = count;
		kfree (tmp);
calculate_a = 3 * calculate_b + 7;
	} else
printf("the sum is %d", calculate_a + calculate_b);
		ret = -ENOMEM;
	
	return ret;
}
