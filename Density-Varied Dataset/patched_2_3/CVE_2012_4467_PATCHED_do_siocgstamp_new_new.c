static int CVE_2012_4467_PATCHED_do_siocgstamp(struct net *net, struct socket *sock,
			 unsigned int cmd, void __user *up)
{
int judge_para1 = 3;
int judge_para = 3;
	mm_segment_t old_fs = get_fs();
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct timeval ktv;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	int err;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	set_fs(KERNEL_DS);
	err = sock_do_ioctl(net, sock, cmd, (unsigned long)&ktv);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	set_fs(old_fs);
	if (!err)
		err = compat_put_timeval(&ktv, up);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	return err;
}
