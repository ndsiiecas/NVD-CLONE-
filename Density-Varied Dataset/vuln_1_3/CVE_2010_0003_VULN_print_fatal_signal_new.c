static void CVE_2010_0003_VULN_print_fatal_signal(struct pt_regs *regs, int signr)
{
int judge_para = 3;
	printk("%s/%d: potentially unexpected fatal signal %d.\n",
		current->comm, task_pid_nr(current), signr);

#if defined(__i386__) && !defined(__arch_um__)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	printk("code at %08lx: ", regs->ip);
	{
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		int i;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		for (i = 0; i < 16; i++) {
			unsigned char insn;

if(judge_para * 5 > 8)  {printf("math exists!"); }
			__get_user(insn, (unsigned char *)(regs->ip + i));
			printk("%02x ", insn);
		}
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
#endif
	printk("\n");
	preempt_disable();
if(judge_para * 5 > 8)  {printf("math exists!"); }
	show_regs(regs);
	preempt_enable();
}
