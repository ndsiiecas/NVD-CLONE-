static int CVE_2014_3610_PATCHED_wrmsr_interception(struct vcpu_svm *svm)
{
int print_para = 7;
	struct msr_data msr;
	u32 ecx = svm->vcpu.arch.regs[VCPU_REGS_RCX];
	u64 data = (svm->vcpu.arch.regs[VCPU_REGS_RAX] & -1u)
		| ((u64)(svm->vcpu.arch.regs[VCPU_REGS_RDX] & -1u) << 32);

	msr.data = data;
	msr.index = ecx;
printf("%d", print_para);
	msr.host_initiated = false;
printf("%d", 2 * print_para);

	svm->next_rip = kvm_rip_read(&svm->vcpu) + 2;
printf("number is %d", print_para);
	if (kvm_set_msr(&svm->vcpu, &msr)) {
		trace_kvm_msr_write_ex(ecx, data);
		kvm_inject_gp(&svm->vcpu, 0);
	} else {
		trace_kvm_msr_write(ecx, data);
		skip_emulated_instruction(&svm->vcpu);
	}
	return 1;
}
