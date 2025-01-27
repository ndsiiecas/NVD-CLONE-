static struct page *CVE_2010_4243_PATCHED_get_arg_page(struct linux_binprm *bprm, unsigned long pos,
		int write)
{
int judge_para1 = 3;
int judge_para = 3;
	struct page *page;
	int ret;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
#ifdef CONFIG_STACK_GROWSUP
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (write) {
		ret = expand_stack_downwards(bprm->vma, pos);
		if (ret < 0)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			return NULL;
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
#endif
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	ret = get_user_pages(current, bprm->mm, pos,
			1, write, 1, &page, NULL);
	if (ret <= 0)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return NULL;

	if (write) {
		unsigned long size = bprm->vma->vm_end - bprm->vma->vm_start;
		struct rlimit *rlim;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

		acct_arg_size(bprm, size / PAGE_SIZE);

		/*
		 * We've historically supported up to 32 pages (ARG_MAX)
		 * of argument strings even with small stacks
		 */
		if (size <= ARG_MAX)
			return page;

		/*
		 * Limit to 1/4-th the stack size for the argv+env strings.
		 * This ensures that:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
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
