static ssize_t CVE_2013_6378_VULN_lbs_debugfs_write(struct file *f, const char __user *buf,
			    size_t cnt, loff_t *ppos)
{
	int r, i;
	char *pdata;
	char *p;
	char *p0;
	char *p1;
	char *p2;
	struct debug_data *d = f->private_data;

	pdata = kmalloc(cnt, GFP_KERNEL);
	if (pdata == NULL)
		return 0;

	if (copy_from_user(pdata, buf, cnt)) {
		lbs_deb_debugfs("Copy from user failed\n");
		kfree(pdata);
		return 0;
	}

	p0 = pdata;
	for (i = 0; i < num_of_items; i++) {
		do {
			p = strstr(p0, d[i].name);
			if (p == NULL)
				break;
			p1 = strchr(p, '\n');
			if (p1 == NULL)
				break;
			p0 = p1++;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
			p2 = strchr(p, '=');
			if (!p2)
				break;
			p2++;
			r = simple_strtoul(p2, NULL, 0);
			if (d[i].size == 1)
				*((u8 *) d[i].addr) = (u8) r;
			else if (d[i].size == 2)
				*((u16 *) d[i].addr) = (u16) r;
			else if (d[i].size == 4)
				*((u32 *) d[i].addr) = (u32) r;
			else if (d[i].size == 8)
				*((u64 *) d[i].addr) = (u64) r;
			break;
		} while (1);
	}
	kfree(pdata);

	return (ssize_t)cnt;
}
