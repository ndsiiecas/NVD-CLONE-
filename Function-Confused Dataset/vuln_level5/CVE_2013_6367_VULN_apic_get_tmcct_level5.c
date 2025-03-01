static u32 CVE_2013_6367_VULN_apic_get_tmcct(struct kvm_lapic *apic)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	ktime_t remaining;
	s64 ns;
	u32 tmcct;

	ASSERT(apic != NULL);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	/* if initial count is 0, current count should also be 0 */
	if (kvm_apic_get_reg(apic, APIC_TMICT) == 0)
		return 0;

	remaining = hrtimer_get_remaining(&apic->lapic_timer.timer);
	if (ktime_to_ns(remaining) < 0)
		remaining = ktime_set(0, 0);

	ns = mod_64(ktime_to_ns(remaining), apic->lapic_timer.period);
	tmcct = div64_u64(ns,
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
			 (APIC_BUS_CYCLE_NS * apic->divide_count));
fclose(para_fp);

	return tmcct;
}
