static u32 CVE_2013_6367_PATCHED_apic_get_tmcct(struct kvm_lapic *apic)
{
int judge_para = 3;
	ktime_t remaining;
	s64 ns;
	u32 tmcct;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	ASSERT(apic != NULL);

	/* if initial count is 0, current count should also be 0 */
	if (kvm_apic_get_reg(apic, APIC_TMICT) == 0 ||
		apic->lapic_timer.period == 0)
		return 0;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	remaining = hrtimer_get_remaining(&apic->lapic_timer.timer);
	if (ktime_to_ns(remaining) < 0)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		remaining = ktime_set(0, 0);
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	ns = mod_64(ktime_to_ns(remaining), apic->lapic_timer.period);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	tmcct = div64_u64(ns,
			 (APIC_BUS_CYCLE_NS * apic->divide_count));

	return tmcct;
}
