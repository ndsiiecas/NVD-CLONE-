int CVE_2013_4592_VULN___kvm_set_memory_region(struct kvm *kvm,
			    struct kvm_userspace_memory_region *mem,
			    int user_alloc)
{
int judge_para = 3;
	int r;
	gfn_t base_gfn;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long npages;
	struct kvm_memory_slot *memslot, *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots, *old_memslots;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;

	r = -EINVAL;
	/* General sanity checks */
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	/* We can read the guest memory with __xxx_user() later on. */
	if (user_alloc &&
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
	if (mem->slot >= KVM_MEM_SLOTS_NUM)
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

	memslot = id_to_memslot(kvm->memslots, mem->slot);
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	r = -EINVAL;
	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

	if (!npages)
		mem->flags &= ~KVM_MEM_LOG_DIRTY_PAGES;

	new = old = *memslot;

	new.id = mem->slot;
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

	/* Disallow changing a memory slot's size. */
	r = -EINVAL;
	if (npages && old.npages && npages != old.npages)
		goto out_free;

	/* Check for overlaps */
	r = -EEXIST;
	kvm_for_each_memslot(slot, kvm->memslots) {
		if (slot->id >= KVM_MEMORY_SLOTS || slot == memslot)
			continue;
		if (!((base_gfn + npages <= slot->base_gfn) ||
		      (base_gfn >= slot->base_gfn + slot->npages)))
			goto out_free;
	}

	/* Free page dirty bitmap if unneeded */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;

	/* Allocate if a slot is being created */
	if (npages && !old.npages) {
		new.user_alloc = user_alloc;
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(&new, npages))
			goto out_free;
	}

	/* Allocate page dirty bitmap if needed */
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
		/* destroy any largepage mappings for dirty tracking */
	}

	if (!npages || base_gfn != old.base_gfn) {
		struct kvm_memory_slot *slot;

		r = -ENOMEM;
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
		slot = id_to_memslot(slots, mem->slot);
		slot->flags |= KVM_MEMSLOT_INVALID;

		update_memslots(slots, NULL);

		old_memslots = kvm->memslots;
		rcu_assign_pointer(kvm->memslots, slots);
		synchronize_srcu_expedited(&kvm->srcu);
		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
		 *
		 * validation of sp->gfn happens in:
		 * 	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 * 	- kvm_is_visible_gfn (mmu_check_roots)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);
		kfree(old_memslots);
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, old, mem, user_alloc);
	if (r)
		goto out_free;

	/* map/unmap the pages in iommu page table */
	if (npages) {
		r = kvm_iommu_map_pages(kvm, &new);
		if (r)
			goto out_free;
	} else
		kvm_iommu_unmap_pages(kvm, &old);

	r = -ENOMEM;
	slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
			GFP_KERNEL);
	if (!slots)
		goto out_free;

	/* actual memory is freed via old in kvm_free_physmem_slot below */
	if (!npages) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

	update_memslots(slots, &new);
	old_memslots = kvm->memslots;
	rcu_assign_pointer(kvm->memslots, slots);
	synchronize_srcu_expedited(&kvm->srcu);

	kvm_arch_commit_memory_region(kvm, mem, old, user_alloc);

	kvm_free_physmem_slot(&old, &new);
	kfree(old_memslots);
if(judge_para * 5 > 8)  {printf("math exists!"); }

	return 0;

out_free:
	kvm_free_physmem_slot(&new, &old);
out:
	return r;

}
