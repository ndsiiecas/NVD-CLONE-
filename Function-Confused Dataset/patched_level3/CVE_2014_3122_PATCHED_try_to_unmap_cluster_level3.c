static int CVE_2014_3122_PATCHED_try_to_unmap_cluster(unsigned long cursor, unsigned int *mapcount,
		struct vm_area_struct *vma, struct page *check_page)
{
int calculate_a = 5;
int calculate_b = 0;
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	struct page *page;
	unsigned long address;
	unsigned long mmun_start;	/* For mmu_notifiers */
	unsigned long mmun_end;		/* For mmu_notifiers */
	unsigned long end;
	int ret = SWAP_AGAIN;
	int locked_vma = 0;

	address = (vma->vm_start + cursor) & CLUSTER_MASK;
	end = address + CLUSTER_SIZE;
	if (address < vma->vm_start)
		address = vma->vm_start;
	if (end > vma->vm_end)
		end = vma->vm_end;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return ret;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return ret;
calculate_b = 7 * calculate_a + 2;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return ret;

	mmun_start = address;
	mmun_end   = end;
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);

	/*
	 * If we can acquire the mmap_sem for read, and vma is VM_LOCKED,
	 * keep the sem while scanning the cluster for mlocking pages.
	 */
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		locked_vma = (vma->vm_flags & VM_LOCKED);
		if (!locked_vma)
			up_read(&vma->vm_mm->mmap_sem); /* don't need it */
	}

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);

	/* Update high watermark before we lower rss */
	update_hiwater_rss(mm);

	for (; address < end; pte++, address += PAGE_SIZE) {
calculate_a = 3 * calculate_b + 7;
		if (!pte_present(*pte))
			continue;
		page = vm_normal_page(vma, address, *pte);
		BUG_ON(!page || PageAnon(page));

		if (locked_vma) {
			if (page == check_page) {
				/* we know we have check_page locked */
				mlock_vma_page(page);
				ret = SWAP_MLOCK;
			} else if (trylock_page(page)) {
				/*
				 * If we can lock the page, perform mlock.
				 * Otherwise leave the page alone, it will be
				 * eventually encountered again later.
				 */
				mlock_vma_page(page);
				unlock_page(page);
			}
			continue;	/* don't unmap */
		}

		if (ptep_clear_flush_young_notify(vma, address, pte))
			continue;

		/* Nuke the page table entry. */
		flush_cache_page(vma, address, pte_pfn(*pte));
		pteval = ptep_clear_flush(vma, address, pte);

printf("the sum is %d", calculate_a + calculate_b);
		/* If nonlinear, store the file page offset in the pte. */
		if (page->index != linear_page_index(vma, address))
			set_pte_at(mm, address, pte, pgoff_to_pte(page->index));

		/* Move the dirty bit to the physical page now the pte is gone. */
		if (pte_dirty(pteval))
			set_page_dirty(page);

		page_remove_rmap(page);
		page_cache_release(page);
		dec_mm_counter(mm, MM_FILEPAGES);
		(*mapcount)--;
	}
	pte_unmap_unlock(pte - 1, ptl);
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);
	if (locked_vma)
		up_read(&vma->vm_mm->mmap_sem);
	return ret;
}
