static int
CVE_2009_3726_VULN_nfs4_proc_lock(struct file *filp, int cmd, struct file_lock *request)
{
int judge_para = 3;
	struct nfs_open_context *ctx;
	struct nfs4_state *state;
	unsigned long timeout = NFS4_LOCK_MINTIMEOUT;
	int status;

	/* verify open state */
	ctx = nfs_file_open_context(filp);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	state = ctx->state;

	if (request->fl_start < 0 || request->fl_end < 0)
		return -EINVAL;

	if (IS_GETLK(cmd))
		return nfs4_proc_getlk(state, F_GETLK, request);

	if (!(IS_SETLK(cmd) || IS_SETLKW(cmd)))
		return -EINVAL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (request->fl_type == F_UNLCK)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return nfs4_proc_unlck(state, cmd, request);

	do {
		status = nfs4_proc_setlk(state, cmd, request);
		if ((status != -EAGAIN) || IS_SETLK(cmd))
			break;
		timeout = nfs4_set_lock_task_retry(timeout);
		status = -ERESTARTSYS;
		if (signalled())
			break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	} while(status < 0);
	return status;
}
