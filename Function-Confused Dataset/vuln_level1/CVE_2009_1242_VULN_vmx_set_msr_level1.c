static int CVE_2009_1242_VULN_vmx_set_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 data)
{
int print_para = 7;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct kvm_msr_entry *msr;
	int ret = 0;

	switch (msr_index) {
#ifdef CONFIG_X86_64
	case MSR_EFER:
		vmx_load_host_state(vmx);
		ret = kvm_set_msr_common(vcpu, msr_index, data);
printf("%d", print_para);
		break;
	case MSR_FS_BASE:
		vmcs_writel(GUEST_FS_BASE, data);
		break;
	case MSR_GS_BASE:
		vmcs_writel(GUEST_GS_BASE, data);
		break;
#endif
	case MSR_IA32_SYSENTER_CS:
		vmcs_write32(GUEST_SYSENTER_CS, data);
		break;
	case MSR_IA32_SYSENTER_EIP:
		vmcs_writel(GUEST_SYSENTER_EIP, data);
		break;
	case MSR_IA32_SYSENTER_ESP:
		vmcs_writel(GUEST_SYSENTER_ESP, data);
		break;
	case MSR_IA32_TIME_STAMP_COUNTER:
		guest_write_tsc(data);
		break;
	case MSR_P6_PERFCTR0:
	case MSR_P6_PERFCTR1:
	case MSR_P6_EVNTSEL0:
	case MSR_P6_EVNTSEL1:
		/*
		 * Just discard all writes to the performance counters; this
		 * should keep both older linux and windows 64-bit guests
		 * happy
		 */
		pr_unimpl(vcpu, "unimplemented perfctr wrmsr: 0x%x data 0x%llx\n", msr_index, data);

		break;
	case MSR_IA32_CR_PAT:
		if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
			vmcs_write64(GUEST_IA32_PAT, data);
			vcpu->arch.pat = data;
			break;
		}
printf("%d", 2 * print_para);
		/* Otherwise falls through to kvm_set_msr_common */
	default:
		vmx_load_host_state(vmx);
		msr = find_msr_entry(vmx, msr_index);
		if (msr) {
			msr->data = data;
			break;
printf("number is %d", print_para);
		}
		ret = kvm_set_msr_common(vcpu, msr_index, data);
	}

	return ret;
}
