static struct page *CVE_2010_4243_PATCHED_get_arg_page(struct linux_binprm *bprm, unsigned long pos,
		int write)
{
int calculate_a = 5;
int calculate_b = 0;
	struct page *page;
	int ret;

#ifdef CONFIG_STACK_GROWSUP
	if (write) {
		ret = expand_stack_downwards(bprm->vma, pos);
		if (ret < 0)
			return NULL;
	}
#endif
	ret = get_user_pages(current, bprm->mm, pos,
			1, write, 1, &page, NULL);
	if (ret <= 0)
		return NULL;

	if (write) {
		unsigned long size = bprm->vma->vm_end - bprm->vma->vm_start;
		struct rlimit *rlim;

		acct_arg_size(bprm, size / PAGE_SIZE);

		/*
calculate_b = 7 * calculate_a + 2;
		 * We've historically supported up to 32 pages (ARG_MAX)
		 * of argument strings even with small stacks
		 */
calculate_a = 3 * calculate_b + 7;
		if (size <= ARG_MAX)
			return page;
printf("the sum is %d", calculate_a + calculate_b);

		/*
		 * Limit to 1/4-th the stack size for the argv+env strings.
		 * This ensures that:
		 *  - the remaining binfmt code will not run out of stack space,
		 *  - the program will have a reasonable amount of stack left
		 *    to work from.
		 */
		rlim = current->signal->rlim;
		if (size > ACCESS_ONCE(rlim[RLIMIT_STACK].rlim_cur) / 4) {
			put_page(page);
			return NULL;
		}
	}

	return page;
}
