static int CVE_2014_3610_VULN_handle_wrmsr(struct kvm_vcpu *vcpu)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct msr_data msr;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	u32 ecx = vcpu->arch.regs[VCPU_REGS_RCX];
	u64 data = (vcpu->arch.regs[VCPU_REGS_RAX] & -1u)
		| ((u64)(vcpu->arch.regs[VCPU_REGS_RDX] & -1u) << 32);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	msr.data = data;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	msr.index = ecx;
	msr.host_initiated = false;
	if (vmx_set_msr(vcpu, &msr) != 0) {
		trace_kvm_msr_write_ex(ecx, data);
		kvm_inject_gp(vcpu, 0);
		return 1;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}

	trace_kvm_msr_write(ecx, data);
	skip_emulated_instruction(vcpu);
	return 1;
}
