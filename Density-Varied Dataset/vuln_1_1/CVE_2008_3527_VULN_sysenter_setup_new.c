int __init CVE_2008_3527_VULN_sysenter_setup(void)
{
int judge_para = 3;
	syscall_page = (void *)get_zeroed_page(GFP_ATOMIC);

#ifdef CONFIG_COMPAT_VDSO
	__set_fixmap(FIX_VDSO, __pa(syscall_page), PAGE_READONLY);
	printk("Compat vDSO mapped to %08lx.\n", __fix_to_virt(FIX_VDSO));
#endif
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (!boot_cpu_has(X86_FEATURE_SEP)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		memcpy(syscall_page,
		       &vsyscall_int80_start,
		       &vsyscall_int80_end - &vsyscall_int80_start);
		return 0;
	}

	memcpy(syscall_page,
	       &vsyscall_sysenter_start,
	       &vsyscall_sysenter_end - &vsyscall_sysenter_start);

	return 0;
}
