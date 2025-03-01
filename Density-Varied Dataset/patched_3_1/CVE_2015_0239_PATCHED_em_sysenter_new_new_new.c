static int CVE_2015_0239_PATCHED_em_sysenter(struct x86_emulate_ctxt *ctxt)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	const struct x86_emulate_ops *ops = ctxt->ops;
	struct desc_struct cs, ss;
	u64 msr_data;
	u16 cs_sel, ss_sel;
	u64 efer = 0;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	ops->get_msr(ctxt, MSR_EFER, &efer);
	/* inject #GP if in real mode */
	if (ctxt->mode == X86EMUL_MODE_REAL)
		return emulate_gp(ctxt, 0);

	/*
	 * Not recognized on AMD in compat mode (but is recognized in legacy
	 * mode).
	 */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if ((ctxt->mode != X86EMUL_MODE_PROT64) && (efer & EFER_LMA)
	    && !vendor_intel(ctxt))
		return emulate_ud(ctxt);

	/* XXX sysenter/sysexit have not been tested in 64bit mode.
	* Therefore, we inject an #UD.
	*/
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return emulate_ud(ctxt);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	setup_syscalls_segments(ctxt, &cs, &ss);

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_CS, &msr_data);
	if ((msr_data & 0xfffc) == 0x0)
		return emulate_gp(ctxt, 0);

	ctxt->eflags &= ~(EFLG_VM | EFLG_IF);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	cs_sel = (u16)msr_data & ~SELECTOR_RPL_MASK;
	ss_sel = cs_sel + 8;
	if (efer & EFER_LMA) {
		cs.d = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		cs.l = 1;
	}

	ops->set_segment(ctxt, cs_sel, &cs, 0, VCPU_SREG_CS);
	ops->set_segment(ctxt, ss_sel, &ss, 0, VCPU_SREG_SS);

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_EIP, &msr_data);
	ctxt->_eip = (efer & EFER_LMA) ? msr_data : (u32)msr_data;

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_ESP, &msr_data);
	*reg_write(ctxt, VCPU_REGS_RSP) = (efer & EFER_LMA) ? msr_data :
							      (u32)msr_data;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return X86EMUL_CONTINUE;
}
