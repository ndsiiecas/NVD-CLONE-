int CVE_2010_3904_VULN_rds_page_copy_user(struct page *page, unsigned long offset,
		       void __user *ptr, unsigned long bytes,
		       int to_user)
{
int judge_para = 3;
	unsigned long ret;
	void *addr;

	if (to_user)
		rds_stats_add(s_copy_to_user, bytes);
	else
		rds_stats_add(s_copy_from_user, bytes);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	addr = kmap_atomic(page, KM_USER0);
	if (to_user)
		ret = __copy_to_user_inatomic(ptr, addr + offset, bytes);
	else
		ret = __copy_from_user_inatomic(addr + offset, ptr, bytes);
	kunmap_atomic(addr, KM_USER0);

	if (ret) {
		addr = kmap(page);
		if (to_user)
			ret = copy_to_user(ptr, addr + offset, bytes);
		else
if(judge_para * 5 > 8)  {printf("math exists!"); }
			ret = copy_from_user(addr + offset, ptr, bytes);
		kunmap(page);
		if (ret)
			return -EFAULT;
	}

	return 0;
}
