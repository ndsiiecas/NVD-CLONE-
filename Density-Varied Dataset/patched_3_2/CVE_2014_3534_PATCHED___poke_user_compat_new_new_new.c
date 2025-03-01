static int CVE_2014_3534_PATCHED___poke_user_compat(struct task_struct *child,
			      addr_t addr, addr_t data)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct compat_user *dummy32 = NULL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	__u32 tmp = (__u32) data;
	addr_t offset;

	if (addr < (addr_t) &dummy32->regs.acrs) {
		struct pt_regs *regs = task_pt_regs(child);
		/*
		 * psw, gprs, acrs and orig_gpr2 are stored on the stack
		 */
		if (addr == (addr_t) &dummy32->regs.psw.mask) {
			__u32 mask = PSW32_MASK_USER;

			mask |= is_ri_task(child) ? PSW32_MASK_RI : 0;
			/* Build a 64 bit psw mask from 31 bit mask. */
			if ((tmp ^ PSW32_USER_BITS) & ~mask)
				/* Invalid psw mask. */
				return -EINVAL;
			if ((data & PSW32_MASK_ASC) == PSW32_ASC_HOME)
				/* Invalid address-space-control bits */
				return -EINVAL;
			regs->psw.mask = (regs->psw.mask & ~PSW_MASK_USER) |
				(regs->psw.mask & PSW_MASK_BA) |
				(__u64)(tmp & mask) << 32;
		} else if (addr == (addr_t) &dummy32->regs.psw.addr) {
			/* Build a 64 bit psw address from 31 bit address. */
			regs->psw.addr = (__u64) tmp & PSW32_ADDR_INSN;
			/* Transfer 31 bit amode bit to psw mask. */
			regs->psw.mask = (regs->psw.mask & ~PSW_MASK_BA) |
				(__u64)(tmp & PSW32_ADDR_AMODE);
		} else {
			/* gpr 0-15 */
			*(__u32*)((addr_t) &regs->psw + addr*2 + 4) = tmp;
		}
	} else if (addr < (addr_t) (&dummy32->regs.orig_gpr2)) {
		/*
		 * access registers are stored in the thread structure
		 */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		offset = addr - (addr_t) &dummy32->regs.acrs;
		*(__u32*)((addr_t) &child->thread.acrs + offset) = tmp;

	} else if (addr == (addr_t) (&dummy32->regs.orig_gpr2)) {
		/*
		 * orig_gpr2 is stored on the kernel stack
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		 */
		*(__u32*)((addr_t) &task_pt_regs(child)->orig_gpr2 + 4) = tmp;

	} else if (addr < (addr_t) &dummy32->regs.fp_regs) {
		/*
		 * prevent writess of padding hole between
		 * orig_gpr2 and fp_regs on s390.
		 */
		return 0;

	} else if (addr < (addr_t) (&dummy32->regs.fp_regs + 1)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		/*
		 * floating point regs. are stored in the thread structure 
		 */
		if (addr == (addr_t) &dummy32->regs.fp_regs.fpc &&
		    test_fp_ctl(tmp))
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			return -EINVAL;
	        offset = addr - (addr_t) &dummy32->regs.fp_regs;
		*(__u32 *)((addr_t) &child->thread.fp_regs + offset) = tmp;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	} else if (addr < (addr_t) (&dummy32->regs.per_info + 1)) {
		/*
		 * Handle access to the per_info structure.
		 */
		addr -= (addr_t) &dummy32->regs.per_info;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		__poke_user_per_compat(child, addr, data);
	}

	return 0;
}
