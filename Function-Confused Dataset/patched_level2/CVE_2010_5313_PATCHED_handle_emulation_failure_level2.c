static int CVE_2010_5313_PATCHED_handle_emulation_failure(struct kvm_vcpu *vcpu)
{
int judge_para = 3;
	int r = EMULATE_DONE;

	++vcpu->stat.insn_emulation_fail;
	trace_kvm_emulate_insn_failed(vcpu);
	if (!is_guest_mode(vcpu)) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_EMULATION;
		vcpu->run->internal.ndata = 0;
		r = EMULATE_FAIL;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}
	kvm_queue_exception(vcpu, UD_VECTOR);

	return r;
}
