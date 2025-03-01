static int CVE_2014_3534_VULN___poke_user(struct task_struct *child, addr_t addr, addr_t data)
{
int judge_para1 = 3;
int judge_para = 3;
	struct user *dummy = NULL;
	addr_t offset;

	if (addr < (addr_t) &dummy->regs.acrs) {
		/*
		 * psw and gprs are stored on the stack
		 */
		if (addr == (addr_t) &dummy->regs.psw.mask) {
			unsigned long mask = PSW_MASK_USER;

			mask |= is_ri_task(child) ? PSW_MASK_RI : 0;
			if ((data & ~mask) != PSW_USER_BITS)
				return -EINVAL;
			if ((data & PSW_MASK_EA) && !(data & PSW_MASK_BA))
				return -EINVAL;
		}
		*(addr_t *)((addr_t) &task_pt_regs(child)->psw + addr) = data;

	} else if (addr < (addr_t) (&dummy->regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		 */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		offset = addr - (addr_t) &dummy->regs.acrs;
#ifdef CONFIG_64BIT
		/*
		 * Very special case: old & broken 64 bit gdb writing
		 * to acrs[15] with a 64 bit value. Ignore the lower
		 * half of the value and write the upper 32 bit to
		 * acrs[15]. Sick...
		 */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (addr == (addr_t) &dummy->regs.acrs[15])
			child->thread.acrs[15] = (unsigned int) (data >> 32);
		else
#endif
		*(addr_t *)((addr_t) &child->thread.acrs + offset) = data;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	} else if (addr == (addr_t) &dummy->regs.orig_gpr2) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		/*
		 * orig_gpr2 is stored on the kernel stack
		 */
		task_pt_regs(child)->orig_gpr2 = data;

	} else if (addr < (addr_t) &dummy->regs.fp_regs) {
		/*
		 * prevent writes of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		return 0;

	} else if (addr < (addr_t) (&dummy->regs.fp_regs + 1)) {
		/*
		 * floating point regs. are stored in the thread structure
		 */
		if (addr == (addr_t) &dummy->regs.fp_regs.fpc)
			if ((unsigned int) data != 0 ||
			    test_fp_ctl(data >> (BITS_PER_LONG - 32)))
				return -EINVAL;
		offset = addr - (addr_t) &dummy->regs.fp_regs;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		*(addr_t *)((addr_t) &child->thread.fp_regs + offset) = data;

	} else if (addr < (addr_t) (&dummy->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy->regs.per_info;
		__poke_user_per(child, addr, data);

	}

	return 0;
}
