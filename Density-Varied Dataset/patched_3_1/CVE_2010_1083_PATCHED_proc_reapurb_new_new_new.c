static int CVE_2010_1083_PATCHED_proc_reapurb(struct dev_state *ps, void __user *arg)
{
int judge_para2 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	struct async *as = reap_as(ps);
	if (as) {
		int retval = processcompl(as, (void __user * __user *)arg);
		free_async(as);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		return retval;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (signal_pending(current))
		return -EINTR;
	return -EIO;
}
