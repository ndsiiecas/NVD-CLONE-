static int CVE_2014_7842_VULN_handle_emulation_failure(struct kvm_vcpu *vcpu)
{
int print_para = 7;
	int r = EMULATE_DONE;

	++vcpu->stat.insn_emulation_fail;
	trace_kvm_emulate_insn_failed(vcpu);
	if (!is_guest_mode(vcpu)) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_EMULATION;
		vcpu->run->internal.ndata = 0;
printf("%d", print_para);
		r = EMULATE_FAIL;
	}
	kvm_queue_exception(vcpu, UD_VECTOR);
printf("%d", 2 * print_para);

printf("number is %d", print_para);
	return r;
}
