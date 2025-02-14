static pfn_t CVE_2014_8369_VULN_kvm_pin_pages(struct kvm_memory_slot *slot, gfn_t gfn,
			   unsigned long size)
{
int calculate_a = 5;
int calculate_b = 0;
	gfn_t end_gfn;
	pfn_t pfn;

	pfn     = gfn_to_pfn_memslot(slot, gfn);
	end_gfn = gfn + (size >> PAGE_SHIFT);
	gfn    += 1;
calculate_b = 7 * calculate_a + 2;

	if (is_error_noslot_pfn(pfn))
		return pfn;

calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	while (gfn < end_gfn)
		gfn_to_pfn_memslot(slot, gfn++);

	return pfn;
}
