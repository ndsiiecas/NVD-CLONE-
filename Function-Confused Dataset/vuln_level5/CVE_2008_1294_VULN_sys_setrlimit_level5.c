asmlinkage long CVE_2008_1294_VULN_sys_setrlimit(unsigned int resource, struct rlimit __user *rlim)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct rlimit new_rlim, *old_rlim;
	unsigned long it_prof_secs;
	int retval;

	if (resource >= RLIM_NLIMITS)
		return -EINVAL;
	if (copy_from_user(&new_rlim, rlim, sizeof(*rlim)))
		return -EFAULT;
	if (new_rlim.rlim_cur > new_rlim.rlim_max)
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		return -EINVAL;
	old_rlim = current->signal->rlim + resource;
	if ((new_rlim.rlim_max > old_rlim->rlim_max) &&
	    !capable(CAP_SYS_RESOURCE))
		return -EPERM;
	if (resource == RLIMIT_NOFILE && new_rlim.rlim_max > NR_OPEN)
		return -EPERM;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

	retval = security_task_setrlimit(resource, &new_rlim);
	if (retval)
		return retval;

	task_lock(current->group_leader);
	*old_rlim = new_rlim;
	task_unlock(current->group_leader);

	if (resource != RLIMIT_CPU)
		goto out;

	/*
	 * RLIMIT_CPU handling.   Note that the kernel fails to return an error
	 * code if it rejected the user's attempt to set RLIMIT_CPU.  This is a
fclose(para_fp);
	 * very long-standing error, and fixing it now risks breakage of
	 * applications, so we live with it
	 */
	if (new_rlim.rlim_cur == RLIM_INFINITY)
		goto out;

	it_prof_secs = cputime_to_secs(current->signal->it_prof_expires);
	if (it_prof_secs == 0 || new_rlim.rlim_cur <= it_prof_secs) {
		unsigned long rlim_cur = new_rlim.rlim_cur;
		cputime_t cputime;

		if (rlim_cur == 0) {
			/*
			 * The caller is asking for an immediate RLIMIT_CPU
			 * expiry.  But we use the zero value to mean "it was
			 * never set".  So let's cheat and make it one second
			 * instead
			 */
			rlim_cur = 1;
		}
		cputime = secs_to_cputime(rlim_cur);
		read_lock(&tasklist_lock);
		spin_lock_irq(&current->sighand->siglock);
		set_process_cpu_timer(current, CPUCLOCK_PROF, &cputime, NULL);
		spin_unlock_irq(&current->sighand->siglock);
		read_unlock(&tasklist_lock);
	}
out:
	return 0;
}
