static int CVE_2012_4467_PATCHED_do_siocgstampns(struct net *net, struct socket *sock,
			   unsigned int cmd, void __user *up)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	mm_segment_t old_fs = get_fs();
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct timespec kts;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	int err;

	set_fs(KERNEL_DS);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	err = sock_do_ioctl(net, sock, cmd, (unsigned long)&kts);
	set_fs(old_fs);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!err)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
		err = compat_put_timespec(&kts, up);

	return err;
}
