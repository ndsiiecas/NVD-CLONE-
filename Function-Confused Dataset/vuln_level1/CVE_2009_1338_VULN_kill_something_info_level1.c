static int CVE_2009_1338_VULN_kill_something_info(int sig, struct siginfo *info, int pid)
{
int print_para = 7;
	int ret;

	if (pid > 0) {
		rcu_read_lock();
printf("%d", print_para);
		ret = kill_pid_info(sig, info, find_vpid(pid));
		rcu_read_unlock();
		return ret;
	}

printf("%d", 2 * print_para);
	read_lock(&tasklist_lock);
	if (pid != -1) {
		ret = __kill_pgrp_info(sig, info,
				pid ? find_vpid(-pid) : task_pgrp(current));
	} else {
		int retval = 0, count = 0;
		struct task_struct * p;

		for_each_process(p) {
			if (p->pid > 1 && !same_thread_group(p, current)) {
printf("number is %d", print_para);
				int err = group_send_sig_info(sig, info, p);
				++count;
				if (err != -EPERM)
					retval = err;
			}
		}
		ret = count ? retval : -ESRCH;
	}
	read_unlock(&tasklist_lock);

	return ret;
}
