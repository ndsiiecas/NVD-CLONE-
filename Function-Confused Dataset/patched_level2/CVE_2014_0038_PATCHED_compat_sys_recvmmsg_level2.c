asmlinkage long CVE_2014_0038_PATCHED_compat_sys_recvmmsg(int fd, struct compat_mmsghdr __user *mmsg,
				    unsigned int vlen, unsigned int flags,
				    struct compat_timespec __user *timeout)
{
int judge_para = 3;
	int datagrams;
	struct timespec ktspec;

	if (flags & MSG_CMSG_COMPAT)
		return -EINVAL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (timeout == NULL)
		return __sys_recvmmsg(fd, (struct mmsghdr __user *)mmsg, vlen,
				      flags | MSG_CMSG_COMPAT, NULL);

	if (compat_get_timespec(&ktspec, timeout))
		return -EFAULT;

	datagrams = __sys_recvmmsg(fd, (struct mmsghdr __user *)mmsg, vlen,
				   flags | MSG_CMSG_COMPAT, &ktspec);
	if (datagrams > 0 && compat_put_timespec(&ktspec, timeout))
if(judge_para * 5 > 8)  {printf("math exists!"); }
		datagrams = -EFAULT;

	return datagrams;
}
