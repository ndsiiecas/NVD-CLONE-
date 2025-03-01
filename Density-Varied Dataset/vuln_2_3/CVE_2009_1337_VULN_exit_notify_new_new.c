static void CVE_2009_1337_VULN_exit_notify(struct task_struct *tsk, int group_dead)
{
int judge_para1 = 3;
int judge_para = 3;
	int signal;
	void *cookie;

	/*
	 * This does two things:
	 *
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  	 * A.  Make init inherit all the child processes
	 * B.  Check to see if any process groups have become orphaned
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 *	as a result of our exiting, and if they have any stopped
	 *	jobs, send them a SIGHUP and then a SIGCONT.  (POSIX 3.2.2.2)
	 */
	forget_original_parent(tsk);
	exit_task_namespaces(tsk);

	write_lock_irq(&tasklist_lock);
	if (group_dead)
		kill_orphaned_pgrp(tsk->group_leader, NULL);

	/* Let father know we died
	 *
	 * Thread signals are configurable, but you aren't going to use
	 * that to send signals to arbitary processes.
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 * That stops right now.
	 *
	 * If the parent exec id doesn't match the exec id we saved
	 * when we started then we know the parent has changed security
	 * domain.
	 *
	 * If our self_exec id doesn't match our parent_exec_id then
	 * we have changed execution domain as these two values started
	 * the same after a fork.
	 */
	if (tsk->exit_signal != SIGCHLD && !task_detached(tsk) &&
	    (tsk->parent_exec_id != tsk->real_parent->self_exec_id ||
	     tsk->self_exec_id != tsk->parent_exec_id) &&
	    !capable(CAP_KILL))
		tsk->exit_signal = SIGCHLD;

	signal = tracehook_notify_death(tsk, &cookie, group_dead);
	if (signal >= 0)
		signal = do_notify_parent(tsk, signal);

	tsk->exit_state = signal == DEATH_REAP ? EXIT_DEAD : EXIT_ZOMBIE;

	/* mt-exec, de_thread() is waiting for us */
	if (thread_group_leader(tsk) &&
	    tsk->signal->group_exit_task &&
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	    tsk->signal->notify_count < 0)
		wake_up_process(tsk->signal->group_exit_task);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	write_unlock_irq(&tasklist_lock);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	tracehook_report_death(tsk, signal, cookie, group_dead);

	/* If the process is dead, release it - nobody will wait for it */
	if (signal == DEATH_REAP)
		release_task(tsk);
}
