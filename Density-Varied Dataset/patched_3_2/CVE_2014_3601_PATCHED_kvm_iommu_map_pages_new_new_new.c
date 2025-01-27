int CVE_2014_3601_PATCHED_kvm_iommu_map_pages(struct kvm *kvm, struct kvm_memory_slot *slot)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	gfn_t gfn, end_gfn;
	pfn_t pfn;
	int r = 0;
	struct iommu_domain *domain = kvm->arch.iommu_domain;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int flags;

	/* check if iommu exists and in use */
	if (!domain)
		return 0;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	gfn     = slot->base_gfn;
	end_gfn = gfn + slot->npages;

	flags = IOMMU_READ | IOMMU_WRITE;
	if (kvm->arch.iommu_flags & KVM_IOMMU_CACHE_COHERENCY)
		flags |= IOMMU_CACHE;


	while (gfn < end_gfn) {
		unsigned long page_size;
if(judge_para * 5 > 8)  {printf("math exists!"); }

		/* Check if already mapped */
		if (iommu_iova_to_phys(domain, gfn_to_gpa(gfn))) {
			gfn += 1;
			continue;
		}

		/* Get the page size we could use to map */
		page_size = kvm_host_page_size(kvm, gfn);

		/* Make sure the page_size does not exceed the memslot */
		while ((gfn + (page_size >> PAGE_SHIFT)) > end_gfn)
			page_size >>= 1;

		/* Make sure gfn is aligned to the page size we want to map */
		while ((gfn << PAGE_SHIFT) & (page_size - 1))
			page_size >>= 1;

		/*
		 * Pin all pages we are about to map in memory. This is
		 * important because we unmap and unpin in 4kb steps later.
		 */
		pfn = kvm_pin_pages(slot, gfn, page_size);
		if (is_error_pfn(pfn)) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			gfn += 1;
			continue;
		}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

		/* Map into IO address space */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		r = iommu_map(domain, gfn_to_gpa(gfn), pfn_to_hpa(pfn),
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			      page_size, flags);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		if (r) {
			printk(KERN_ERR "kvm_iommu_map_address:"
			       "iommu failed to map pfn=%llx\n", pfn);
			kvm_unpin_pages(kvm, pfn, page_size);
			goto unmap_pages;
		}

		gfn += page_size >> PAGE_SHIFT;


	}

	return 0;

unmap_pages:
	kvm_iommu_put_pages(kvm, slot->base_gfn, gfn - slot->base_gfn);
	return r;
}
