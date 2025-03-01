int CVE_2009_2287_PATCHED_kvm_arch_vcpu_ioctl_set_sregs(struct kvm_vcpu *vcpu,
				  struct kvm_sregs *sregs)
{
int origin_a, origin_aa[4];
	int mmu_reset_needed = 0;
	int i, pending_vec, max_bits;
	struct descriptor_table dt;

	vcpu_load(vcpu);

	dt.limit = sregs->idt.limit;
	dt.base = sregs->idt.base;
	kvm_x86_ops->set_idt(vcpu, &dt);
	dt.limit = sregs->gdt.limit;
	dt.base = sregs->gdt.base;
	kvm_x86_ops->set_gdt(vcpu, &dt);

	vcpu->arch.cr2 = sregs->cr2;
	mmu_reset_needed |= vcpu->arch.cr3 != sregs->cr3;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

	down_read(&vcpu->kvm->slots_lock);
	if (gfn_to_memslot(vcpu->kvm, sregs->cr3 >> PAGE_SHIFT))
		vcpu->arch.cr3 = sregs->cr3;
	else
		set_bit(KVM_REQ_TRIPLE_FAULT, &vcpu->requests);
	up_read(&vcpu->kvm->slots_lock);

	kvm_set_cr8(vcpu, sregs->cr8);

	mmu_reset_needed |= vcpu->arch.shadow_efer != sregs->efer;
	kvm_x86_ops->set_efer(vcpu, sregs->efer);
	kvm_set_apic_base(vcpu, sregs->apic_base);
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}

	kvm_x86_ops->decache_cr4_guest_bits(vcpu);

	mmu_reset_needed |= vcpu->arch.cr0 != sregs->cr0;
	kvm_x86_ops->set_cr0(vcpu, sregs->cr0);
	vcpu->arch.cr0 = sregs->cr0;

	mmu_reset_needed |= vcpu->arch.cr4 != sregs->cr4;
	kvm_x86_ops->set_cr4(vcpu, sregs->cr4);
	if (!is_long_mode(vcpu) && is_pae(vcpu))
		load_pdptrs(vcpu, vcpu->arch.cr3);

	if (mmu_reset_needed)
		kvm_mmu_reset_context(vcpu);

	if (!irqchip_in_kernel(vcpu->kvm)) {
		memcpy(vcpu->arch.irq_pending, sregs->interrupt_bitmap,
		       sizeof vcpu->arch.irq_pending);
		vcpu->arch.irq_summary = 0;
		for (i = 0; i < ARRAY_SIZE(vcpu->arch.irq_pending); ++i)
			if (vcpu->arch.irq_pending[i])
				__set_bit(i, &vcpu->arch.irq_summary);
	} else {
		max_bits = (sizeof sregs->interrupt_bitmap) << 3;
		pending_vec = find_first_bit(
			(const unsigned long *)sregs->interrupt_bitmap,
			max_bits);
		/* Only pending external irq is handled here */
		if (pending_vec < max_bits) {
			kvm_x86_ops->set_irq(vcpu, pending_vec);
			pr_debug("Set back pending irq %d\n",
				 pending_vec);
		}
		kvm_pic_clear_isr_ack(vcpu->kvm);
	}

	kvm_set_segment(vcpu, &sregs->cs, VCPU_SREG_CS);
	kvm_set_segment(vcpu, &sregs->ds, VCPU_SREG_DS);
	kvm_set_segment(vcpu, &sregs->es, VCPU_SREG_ES);
	kvm_set_segment(vcpu, &sregs->fs, VCPU_SREG_FS);
	kvm_set_segment(vcpu, &sregs->gs, VCPU_SREG_GS);
	kvm_set_segment(vcpu, &sregs->ss, VCPU_SREG_SS);

	kvm_set_segment(vcpu, &sregs->tr, VCPU_SREG_TR);
	kvm_set_segment(vcpu, &sregs->ldt, VCPU_SREG_LDTR);

	/* Older userspace won't unhalt the vcpu on reset. */
	if (vcpu->vcpu_id == 0 && kvm_rip_read(vcpu) == 0xfff0 &&
	    sregs->cs.selector == 0xf000 && sregs->cs.base == 0xffff0000 &&
	    !(vcpu->arch.cr0 & X86_CR0_PE))
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		vcpu->arch.mp_state = KVM_MP_STATE_RUNNABLE;

	vcpu_put(vcpu);

	return 0;
}
