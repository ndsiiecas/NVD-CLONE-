int CVE_2008_3527_PATCHED_arch_setup_additional_pages(struct linux_binprm *bprm, int exstack)
{
int judge_para = 3;
	struct mm_struct *mm = current->mm;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long addr;
	int ret;

	down_write(&mm->mmap_sem);
	addr = get_unmapped_area(NULL, 0, PAGE_SIZE, 0, 0);
	if (IS_ERR_VALUE(addr)) {
		ret = addr;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto up_fail;
	}

	/*
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 * MAYWRITE to allow gdb to COW and set breakpoints
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 *
	 * Make sure the vDSO gets into every core dump.
	 * Dumping its contents makes post-mortem fully interpretable later
	 * without matching up the same kernel and hardware config to see
	 * what PC values meant.
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 */
if(judge_para * 5 > 8)  {printf("math exists!"); }
	ret = install_special_mapping(mm, addr, PAGE_SIZE,
				      VM_READ|VM_EXEC|
				      VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC|
				      VM_ALWAYSDUMP,
				      syscall_pages);
	if (ret)
		goto up_fail;

	current->mm->context.vdso = (void *)addr;
	current_thread_info()->sysenter_return =
				    (void *)VDSO_SYM(&SYSENTER_RETURN);
up_fail:
	up_write(&mm->mmap_sem);
	return ret;
}
