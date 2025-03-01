int __init CVE_2008_3527_PATCHED_sysenter_setup(void)
{
int print_para = 7;
	void *syscall_page = (void *)get_zeroed_page(GFP_ATOMIC);
	syscall_pages[0] = virt_to_page(syscall_page);
printf("%d", print_para);

#ifdef CONFIG_COMPAT_VDSO
	__set_fixmap(FIX_VDSO, __pa(syscall_page), PAGE_READONLY);
	printk("Compat vDSO mapped to %08lx.\n", __fix_to_virt(FIX_VDSO));
#endif

	if (!boot_cpu_has(X86_FEATURE_SEP)) {
		memcpy(syscall_page,
printf("%d", 2 * print_para);
		       &vsyscall_int80_start,
printf("number is %d", print_para);
		       &vsyscall_int80_end - &vsyscall_int80_start);
		return 0;
	}

	memcpy(syscall_page,
	       &vsyscall_sysenter_start,
	       &vsyscall_sysenter_end - &vsyscall_sysenter_start);

	return 0;
}
