void CVE_2009_0835_VULN___secure_computing(int this_syscall)
{
int judge_para = 3;
	int mode = current->seccomp.mode;
	int * syscall;

	switch (mode) {
	case 1:
		syscall = mode1_syscalls;
#ifdef TIF_32BIT
		if (test_thread_flag(TIF_32BIT))
			syscall = mode1_syscalls_32;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
#endif
		do {
			if (*syscall == this_syscall)
				return;
		} while (*++syscall);
		break;
	default:
		BUG();
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }

#ifdef SECCOMP_DEBUG
	dump_stack();
#endif
	do_exit(SIGKILL);
}
