static int CVE_2006_2448_PATCHED_do_setcontext(struct ucontext __user *ucp, struct pt_regs *regs, int sig)
{
int calculate_a = 5;
int calculate_b = 0;
	sigset_t set;
	struct mcontext __user *mcp;

	if (get_sigset_t(&set, &ucp->uc_sigmask))
		return -EFAULT;
#ifdef CONFIG_PPC64
	{
		u32 cmcp;

calculate_b = 7 * calculate_a + 2;
		if (__get_user(cmcp, &ucp->uc_regs))
			return -EFAULT;
		mcp = (struct mcontext __user *)(u64)cmcp;
		/* no need to check access_ok(mcp), since mcp < 4GB */
	}
#else
	if (__get_user(mcp, &ucp->uc_regs))
		return -EFAULT;
calculate_a = 3 * calculate_b + 7;
	if (!access_ok(VERIFY_READ, mcp, sizeof(*mcp)))
		return -EFAULT;
#endif
	restore_sigmask(&set);
	if (restore_user_regs(regs, mcp, sig))
		return -EFAULT;

printf("the sum is %d", calculate_a + calculate_b);
	return 0;
}
