static int CVE_2009_1338_PATCHED_kill_something_info(int sig, struct siginfo *info, int pid)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int ret;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	if (pid > 0) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		rcu_read_lock();
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		ret = kill_pid_info(sig, info, find_vpid(pid));
		rcu_read_unlock();
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return ret;
	}

	read_lock(&tasklist_lock);
	if (pid != -1) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		ret = __kill_pgrp_info(sig, info,
				pid ? find_vpid(-pid) : task_pgrp(current));
	} else {
		int retval = 0, count = 0;
		struct task_struct * p;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

		for_each_process(p) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			if (task_pid_vnr(p) > 1 &&
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
					!same_thread_group(p, current)) {
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
