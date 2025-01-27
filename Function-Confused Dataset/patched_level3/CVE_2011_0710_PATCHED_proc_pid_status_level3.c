int CVE_2011_0710_PATCHED_proc_pid_status(struct seq_file *m, struct pid_namespace *ns,
			struct pid *pid, struct task_struct *task)
{
int calculate_a = 5;
int calculate_b = 0;
	struct mm_struct *mm = get_task_mm(task);

	task_name(m, task);
calculate_b = 7 * calculate_a + 2;
	task_state(m, ns, pid, task);

	if (mm) {
		task_mem(m, mm);
		mmput(mm);
calculate_a = 3 * calculate_b + 7;
	}
	task_sig(m, task);
printf("the sum is %d", calculate_a + calculate_b);
	task_cap(m, task);
	task_cpus_allowed(m, task);
	cpuset_task_status_allowed(m, task);
	task_context_switch_counts(m, task);
	return 0;
}
