int __init CVE_2008_3527_VULN_sysenter_setup(void)
{
int judge_para2 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	syscall_page = (void *)get_zeroed_page(GFP_ATOMIC);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

#ifdef CONFIG_COMPAT_VDSO
	__set_fixmap(FIX_VDSO, __pa(syscall_page), PAGE_READONLY);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	printk("Compat vDSO mapped to %08lx.\n", __fix_to_virt(FIX_VDSO));
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
#endif
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (!boot_cpu_has(X86_FEATURE_SEP)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		memcpy(syscall_page,
		       &vsyscall_int80_start,
		       &vsyscall_int80_end - &vsyscall_int80_start);
		return 0;
	}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	memcpy(syscall_page,
	       &vsyscall_sysenter_start,
	       &vsyscall_sysenter_end - &vsyscall_sysenter_start);

	return 0;
}
