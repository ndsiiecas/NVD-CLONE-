int CVE_2014_3601_VULN_kvm_iommu_map_pages(struct kvm *kvm, struct kvm_memory_slot *slot)
{
int origin_a, origin_aa[4];
	gfn_t gfn, end_gfn;
	pfn_t pfn;
	int r = 0;
	struct iommu_domain *domain = kvm->arch.iommu_domain;
	int flags;

	/* check if iommu exists and in use */
	if (!domain)
		return 0;

	gfn     = slot->base_gfn;
	end_gfn = gfn + slot->npages;

	flags = IOMMU_READ | IOMMU_WRITE;
	if (kvm->arch.iommu_flags & KVM_IOMMU_CACHE_COHERENCY)
		flags |= IOMMU_CACHE;


	while (gfn < end_gfn) {
		unsigned long page_size;

		/* Check if already mapped */
		if (iommu_iova_to_phys(domain, gfn_to_gpa(gfn))) {
			gfn += 1;
			continue;
		}

		/* Get the page size we could use to map */
		page_size = kvm_host_page_size(kvm, gfn);

origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		/* Make sure the page_size does not exceed the memslot */
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		while ((gfn + (page_size >> PAGE_SHIFT)) > end_gfn)
			page_size >>= 1;

		/* Make sure gfn is aligned to the page size we want to map */
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		while ((gfn << PAGE_SHIFT) & (page_size - 1))
			page_size >>= 1;

		/*
		 * Pin all pages we are about to map in memory. This is
		 * important because we unmap and unpin in 4kb steps later.
		 */
		pfn = kvm_pin_pages(slot, gfn, page_size);
		if (is_error_pfn(pfn)) {
			gfn += 1;
			continue;
		}

		/* Map into IO address space */
		r = iommu_map(domain, gfn_to_gpa(gfn), pfn_to_hpa(pfn),
			      page_size, flags);
		if (r) {
			printk(KERN_ERR "kvm_iommu_map_address:"
			       "iommu failed to map pfn=%llx\n", pfn);
			goto unmap_pages;
		}

		gfn += page_size >> PAGE_SHIFT;


	}

	return 0;

unmap_pages:
	kvm_iommu_put_pages(kvm, slot->base_gfn, gfn);
	return r;
}
