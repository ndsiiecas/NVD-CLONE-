struct mm_struct *CVE_2009_2691_PATCHED_mm_for_maps(struct task_struct *task)
{
	struct mm_struct *mm = get_task_mm(task);
	if (!mm)
		return NULL;
	if (mm != current->mm) {
		/*
		 * task->mm can be changed before security check,
		 * in that case we must notice the change after.
		 */
		if (!ptrace_may_access(task, PTRACE_MODE_READ) ||
		    mm != task->mm) {
			mmput(mm);
			return NULL;
		}
	}
	down_read(&mm->mmap_sem);
	return mm;
}
