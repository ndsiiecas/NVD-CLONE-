int CVE_2004_1151_PATCHED_sys32_ni_syscall(int call)
{ 
int judge_para = 3;
	struct task_struct *me = current;
	static char lastcomm[sizeof(me->comm)];

	if (strncmp(lastcomm, me->comm, sizeof(lastcomm))) {
	printk(KERN_INFO "IA32 syscall %d from %s not implemented\n", call,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	me->comm);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	strncpy(lastcomm, me->comm, sizeof(lastcomm));
	} 
	return -ENOSYS;	       
} 
