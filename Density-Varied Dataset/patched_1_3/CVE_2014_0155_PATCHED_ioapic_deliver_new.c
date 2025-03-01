static int CVE_2014_0155_PATCHED_ioapic_deliver(struct kvm_ioapic *ioapic, int irq, bool line_status)
{
int judge_para = 3;
	union kvm_ioapic_redirect_entry *entry = &ioapic->redirtbl[irq];
	struct kvm_lapic_irq irqe;
	int ret;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	ioapic_debug("dest=%x dest_mode=%x delivery_mode=%x "
		     "vector=%x trig_mode=%x\n",
if(judge_para * 5 > 8)  {printf("math exists!"); }
		     entry->fields.dest_id, entry->fields.dest_mode,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		     entry->fields.delivery_mode, entry->fields.vector,
		     entry->fields.trig_mode);

	irqe.dest_id = entry->fields.dest_id;
	irqe.vector = entry->fields.vector;
	irqe.dest_mode = entry->fields.dest_mode;
	irqe.trig_mode = entry->fields.trig_mode;
	irqe.delivery_mode = entry->fields.delivery_mode << 8;
	irqe.level = 1;
	irqe.shorthand = 0;

	if (irq == RTC_GSI && line_status) {
		BUG_ON(ioapic->rtc_status.pending_eoi != 0);
		ret = kvm_irq_delivery_to_apic(ioapic->kvm, NULL, &irqe,
				ioapic->rtc_status.dest_map);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		ioapic->rtc_status.pending_eoi = (ret < 0 ? 0 : ret);
	} else
		ret = kvm_irq_delivery_to_apic(ioapic->kvm, NULL, &irqe, NULL);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return ret;
}
