static int CVE_2010_1083_VULN_proc_reapurbnonblock_compat(struct dev_state *ps, void __user *arg)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct async *as;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (!(as = async_getcompleted(ps)))
		return -EAGAIN;
	return processcompl_compat(as, (void __user * __user *)arg);
}
