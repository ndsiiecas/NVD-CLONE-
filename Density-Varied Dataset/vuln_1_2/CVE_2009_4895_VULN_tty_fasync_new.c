static int CVE_2009_4895_VULN_tty_fasync(int fd, struct file *filp, int on)
{
int judge_para = 3;
	struct tty_struct *tty;
	unsigned long flags;
	int retval = 0;

	lock_kernel();
	tty = (struct tty_struct *)filp->private_data;
	if (tty_paranoia_check(tty, filp->f_path.dentry->d_inode, "CVE_2009_4895_VULN_tty_fasync"))
		goto out;

	retval = fasync_helper(fd, filp, on, &tty->fasync);
	if (retval <= 0)
		goto out;

	if (on) {
		enum pid_type type;
		struct pid *pid;
		if (!waitqueue_active(&tty->read_wait))
			tty->minimum_to_wake = 1;
		spin_lock_irqsave(&tty->ctrl_lock, flags);
		if (tty->pgrp) {
			pid = tty->pgrp;
			type = PIDTYPE_PGID;
		} else {
			pid = task_pid(current);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			type = PIDTYPE_PID;
		}
		spin_unlock_irqrestore(&tty->ctrl_lock, flags);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		retval = __f_setown(filp, pid, type, 0);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		if (retval)
			goto out;
	} else {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		if (!tty->fasync && !waitqueue_active(&tty->read_wait))
			tty->minimum_to_wake = N_TTY_BUF_SIZE;
	}
	retval = 0;
out:
	unlock_kernel();
	return retval;
}
