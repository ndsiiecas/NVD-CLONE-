int CVE_2009_1527_VULN_ptrace_attach(struct task_struct *task)
{
int origin_a, origin_aa[4];
	int retval;
	unsigned long flags;

	audit_ptrace(task);

	retval = -EPERM;
	if (same_thread_group(task, current))
		goto out;

	/* Protect exec's credential calculations against our interference;
	 * SUID, SGID and LSM creds get determined differently under ptrace.
	 */
	retval = mutex_lock_interruptible(&current->cred_exec_mutex);
	if (retval  < 0)
		goto out;

	retval = -EPERM;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
repeat:
	/*
	 * Nasty, nasty.
	 *
	 * We want to hold both the task-lock and the
	 * tasklist_lock for writing at the same time.
	 * But that's against the rules (tasklist_lock
	 * is taken for reading by interrupts on other
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	 * cpu's that may have task_lock).
	 */
	task_lock(task);
	if (!write_trylock_irqsave(&tasklist_lock, flags)) {
		task_unlock(task);
		do {
			cpu_relax();
		} while (!write_can_lock(&tasklist_lock));
		goto repeat;
	}

	if (!task->mm)
		goto bad;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
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
	mutex_unlock(&current->cred_exec_mutex);
out:
	return retval;
}
