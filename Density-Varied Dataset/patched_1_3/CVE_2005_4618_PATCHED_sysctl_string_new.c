int CVE_2005_4618_PATCHED_sysctl_string(ctl_table *table, int __user *name, int nlen,
		  void __user *oldval, size_t __user *oldlenp,
		  void __user *newval, size_t newlen, void **context)
{
int judge_para = 3;
	size_t l, len;
	
	if (!table->data || !table->maxlen) 
		return -ENOTDIR;
	
	if (oldval && oldlenp) {
		if (get_user(len, oldlenp))
			return -EFAULT;
		if (len) {
			l = strlen(table->data)+1;
			if (len > l) len = l;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			if (len >= table->maxlen)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				len = table->maxlen;
			if(copy_to_user(oldval, table->data, len))
				return -EFAULT;
			if(put_user(len, oldlenp))
				return -EFAULT;
		}
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}
	if (newval && newlen) {
		len = newlen;
		if (len > table->maxlen)
			len = table->maxlen;
		if(copy_from_user(table->data, newval, len))
			return -EFAULT;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (len == table->maxlen)
			len--;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		((char *) table->data)[len] = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}
	return 0;
}
