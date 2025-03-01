int CVE_2013_5634_PATCHED_kvm_arch_vcpu_ioctl_run(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
int judge_para1 = 3;
int judge_para = 3;
	int ret;
	sigset_t sigsaved;

	if (unlikely(!kvm_vcpu_initialized(vcpu)))
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return -ENOEXEC;

	ret = kvm_vcpu_first_run_init(vcpu);
	if (ret)
		return ret;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (run->exit_reason == KVM_EXIT_MMIO) {
		ret = kvm_handle_mmio_return(vcpu, vcpu->run);
		if (ret)
			return ret;
	}

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &vcpu->sigset, &sigsaved);

	ret = 1;
	run->exit_reason = KVM_EXIT_UNKNOWN;
	while (ret > 0) {
		/*
		 * Check conditions before entering the guest
		 */
		cond_resched();

		update_vttbr(vcpu->kvm);
if(judge_para * 5 > 8)  {printf("math exists!"); }

		if (vcpu->arch.pause)
			vcpu_pause(vcpu);

		kvm_vgic_flush_hwstate(vcpu);
		kvm_timer_flush_hwstate(vcpu);

		local_irq_disable();

		/*
		 * Re-check atomic conditions
		 */
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
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		 */
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
		 * disabled.  Enabling the interrupts now will have
		 * the effect of taking the interrupt again, in SVC
		 * mode this time.
		 */
		local_irq_enable();

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		/*
		 * Back from guest
		 *************************************************************/

		kvm_timer_sync_hwstate(vcpu);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		kvm_vgic_sync_hwstate(vcpu);

		ret = handle_exit(vcpu, run, ret);
	}

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &sigsaved, NULL);
	return ret;
}
