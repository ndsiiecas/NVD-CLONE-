static ssize_t CVE_2009_2846_VULN_eisa_eeprom_read(struct file * file,
			      char __user *buf, size_t count, loff_t *ppos )
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	unsigned char *tmp;
	ssize_t ret;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	int i;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	
	if (*ppos >= HPEE_MAX_LENGTH)
		return 0;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	
	count = *ppos + count < HPEE_MAX_LENGTH ? count : HPEE_MAX_LENGTH - *ppos;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	tmp = kmalloc(count, GFP_KERNEL);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (tmp) {
		for (i = 0; i < count; i++)
			tmp[i] = readb(eisa_eeprom_addr+(*ppos)++);

		if (copy_to_user (buf, tmp, count))
			ret = -EFAULT;
		else
			ret = count;
		kfree (tmp);
	} else
		ret = -ENOMEM;
	
	return ret;
}
