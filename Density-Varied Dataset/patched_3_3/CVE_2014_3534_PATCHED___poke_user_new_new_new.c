static int CVE_2014_3534_PATCHED___poke_user(struct task_struct *child, addr_t addr, addr_t data)
{
int judge_para2 = 3;
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
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

			mask |= is_ri_task(child) ? PSW_MASK_RI : 0;
			if ((data ^ PSW_USER_BITS) & ~mask)
				/* Invalid psw mask. */
				return -EINVAL;
			if ((data & PSW_MASK_ASC) == PSW_ASC_HOME)
				/* Invalid address-space-control bits */
				return -EINVAL;
			if ((data & PSW_MASK_EA) && !(data & PSW_MASK_BA))
				/* Invalid addressing mode bits */
				return -EINVAL;
		}
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		*(addr_t *)((addr_t) &task_pt_regs(child)->psw + addr) = data;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	} else if (addr < (addr_t) (&dummy->regs.orig_gpr2)) {
		/*
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		 * access registers are stored in the thread structure
		 */
		offset = addr - (addr_t) &dummy->regs.acrs;
#ifdef CONFIG_64BIT
		/*
		 * Very special case: old & broken 64 bit gdb writing
		 * to acrs[15] with a 64 bit value. Ignore the lower
		 * half of the value and write the upper 32 bit to
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		 * acrs[15]. Sick...
		 */
		if (addr == (addr_t) &dummy->regs.acrs[15])
			child->thread.acrs[15] = (unsigned int) (data >> 32);
		else
#endif
		*(addr_t *)((addr_t) &child->thread.acrs + offset) = data;

	} else if (addr == (addr_t) &dummy->regs.orig_gpr2) {
		/*
		 * orig_gpr2 is stored on the kernel stack
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
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
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		 */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		if (addr == (addr_t) &dummy->regs.fp_regs.fpc)
			if ((unsigned int) data != 0 ||
			    test_fp_ctl(data >> (BITS_PER_LONG - 32)))
				return -EINVAL;
		offset = addr - (addr_t) &dummy->regs.fp_regs;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		*(addr_t *)((addr_t) &child->thread.fp_regs + offset) = data;

	} else if (addr < (addr_t) (&dummy->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		addr -= (addr_t) &dummy->regs.per_info;
		__poke_user_per(child, addr, data);

	}

	return 0;
}
