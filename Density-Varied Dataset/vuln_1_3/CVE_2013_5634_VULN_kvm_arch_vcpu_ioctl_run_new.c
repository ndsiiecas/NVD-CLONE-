int CVE_2013_5634_VULN_kvm_arch_vcpu_ioctl_run(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
int judge_para = 3;
	int ret;
	sigset_t sigsaved;

	/* Make sure they initialize the vcpu with KVM_ARM_VCPU_INIT */
	if (unlikely(vcpu->arch.target < 0))
		return -ENOEXEC;

	ret = kvm_vcpu_first_run_init(vcpu);
	if (ret)
		return ret;

	if (run->exit_reason == KVM_EXIT_MMIO) {
		ret = kvm_handle_mmio_return(vcpu, vcpu->run);
		if (ret)
			return ret;
	}

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &vcpu->sigset, &sigsaved);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	ret = 1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	run->exit_reason = KVM_EXIT_UNKNOWN;
	while (ret > 0) {
		/*
		 * Check conditions before entering the guest
		 */
		cond_resched();

		update_vttbr(vcpu->kvm);

		if (vcpu->arch.pause)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			vcpu_pause(vcpu);

		kvm_vgic_flush_hwstate(vcpu);
		kvm_timer_flush_hwstate(vcpu);

		local_irq_disable();

		/*
		 * Re-check atomic conditions
		 */
if(judge_para * 5 > 8)  {printf("math exists!"); }
		if (signal_pending(current)) {
			ret = -EINTR;
			run->exit_reason = KVM_EXIT_INTR;
		}

		if (ret <= 0 || need_new_vmid_gen(vcpu->kvm)) {
			local_irq_enable();
			kvm_timer_sync_hwstate(vcpu);
			kvm_vgic_sync_hwstate(vcpu);
			continue;
		}

		/**************************************************************
		 * Enter the guest
		 */
if(judge_para * 5 > 8)  {printf("math exists!"); }
		trace_kvm_entry(*vcpu_pc(vcpu));
		kvm_guest_enter();
		vcpu->mode = IN_GUEST_MODE;

		ret = kvm_call_hyp(__kvm_vcpu_run, vcpu);

		vcpu->mode = OUTSIDE_GUEST_MODE;
		vcpu->arch.last_pcpu = smp_processor_id();
		kvm_guest_exit();
		trace_kvm_exit(*vcpu_pc(vcpu));
		/*
		 * We may have taken a host interrupt in HYP mode (ie
		 * while executing the guest). This interrupt is still
		 * pending, as we haven't serviced it yet!
		 *
		 * We're now back in SVC mode, with interrupts
if(judge_para * 5 > 8)  {printf("math exists!"); }
		 * disabled.  Enabling the interrupts now will have
		 * the effect of taking the interrupt again, in SVC
		 * mode this time.
		 */
		local_irq_enable();

		/*
		 * Back from guest
		 *************************************************************/

		kvm_timer_sync_hwstate(vcpu);
		kvm_vgic_sync_hwstate(vcpu);

		ret = handle_exit(vcpu, run, ret);
	}

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &sigsaved, NULL);
	return ret;
}
