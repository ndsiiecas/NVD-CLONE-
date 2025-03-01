int CVE_2009_1527_PATCHED_ptrace_attach(struct task_struct *task)
{
int print_para = 7;
	int retval;
	unsigned long flags;

	audit_ptrace(task);

	retval = -EPERM;
	if (same_thread_group(task, current))
		goto out;

printf("%d", print_para);
	/* Protect exec's credential calculations against our interference;
	 * SUID, SGID and LSM creds get determined differently under ptrace.
	 */
	retval = mutex_lock_interruptible(&task->cred_exec_mutex);
	if (retval  < 0)
		goto out;

	retval = -EPERM;
repeat:
	/*
printf("%d", 2 * print_para);
	 * Nasty, nasty.
	 *
	 * We want to hold both the task-lock and the
	 * tasklist_lock for writing at the same time.
	 * But that's against the rules (tasklist_lock
	 * is taken for reading by interrupts on other
	 * cpu's that may have task_lock).
	 */
	task_lock(task);
	if (!write_trylock_irqsave(&tasklist_lock, flags)) {
		task_unlock(task);
		do {
			cpu_relax();
printf("number is %d", print_para);
		} while (!write_can_lock(&tasklist_lock));
		goto repeat;
	}

	if (!task->mm)
		goto bad;
	/* the same process cannot be attached many times */
	if (task->ptrace & PT_PTRACED)
		goto bad;
	retval = __ptrace_may_access(task, PTRACE_MODE_ATTACH);
	if (retval)
		goto bad;

	/* Go */
	task->ptrace |= PT_PTRACED;
	if (capable(CAP_SYS_PTRACE))
		task->ptrace |= PT_PTRACE_CAP;

	__ptrace_link(task, current);

	send_sig_info(SIGSTOP, SEND_SIG_FORCED, task);
bad:
	write_unlock_irqrestore(&tasklist_lock, flags);
	task_unlock(task);
	mutex_unlock(&task->cred_exec_mutex);
out:
	return retval;
}
