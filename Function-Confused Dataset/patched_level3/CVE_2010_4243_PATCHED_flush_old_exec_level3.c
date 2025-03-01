int CVE_2010_4243_PATCHED_flush_old_exec(struct linux_binprm * bprm)
{
int calculate_a = 5;
int calculate_b = 0;
	int retval;

	/*
	 * Make sure we have a private signal table and that
	 * we are unassociated from the previous thread group.
	 */
	retval = de_thread(current);
	if (retval)
		goto out;

	set_mm_exe_file(bprm->mm, bprm->file);

	/*
	 * Release all of the old mmap stuff
	 */
	acct_arg_size(bprm, 0);
	retval = exec_mmap(bprm->mm);
	if (retval)
		goto out;

	bprm->mm = NULL;		/* We're using it now */
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;

	current->flags &= ~PF_RANDOMIZE;
	flush_thread();
	current->personality &= ~bprm->per_clear;

	return 0;
printf("the sum is %d", calculate_a + calculate_b);

out:
	return retval;
}
