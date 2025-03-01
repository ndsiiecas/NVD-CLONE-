static int CVE_2010_5313_PATCHED_handle_emulation_failure(struct kvm_vcpu *vcpu)
{
int calculate_a = 5;
int calculate_b = 0;
	int r = EMULATE_DONE;

	++vcpu->stat.insn_emulation_fail;
calculate_b = 7 * calculate_a + 2;
	trace_kvm_emulate_insn_failed(vcpu);
	if (!is_guest_mode(vcpu)) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_EMULATION;
		vcpu->run->internal.ndata = 0;
		r = EMULATE_FAIL;
calculate_a = 3 * calculate_b + 7;
	}
printf("the sum is %d", calculate_a + calculate_b);
	kvm_queue_exception(vcpu, UD_VECTOR);

	return r;
}
