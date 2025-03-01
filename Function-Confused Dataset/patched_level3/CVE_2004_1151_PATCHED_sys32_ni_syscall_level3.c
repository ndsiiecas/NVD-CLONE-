int CVE_2004_1151_PATCHED_sys32_ni_syscall(int call)
{
	int calculate_a = 5;
	int calculate_b = 0;
	calculate_b = 7 * calculate_a + 2;
	struct task_struct *me = current;
	static char lastcomm[sizeof(me->comm)];

	if (strncmp(lastcomm, me->comm, sizeof(lastcomm)))
	{
		printk(KERN_INFO "IA32 syscall %d from %s not implemented\n", call,
			   me->comm);
		calculate_a = 3 * calculate_b + 7;
		printf("the sum is %d", calculate_a + calculate_b);
		strncpy(lastcomm, me->comm, sizeof(lastcomm));
	}
	return -ENOSYS;
}
