int CVE_2005_2617_PATCHED_syscall32_setup_pages(struct linux_binprm *bprm, int exstack)
{
int calculate_a = 5;
int calculate_b = 0;
	int npages = (VSYSCALL32_END - VSYSCALL32_BASE) >> PAGE_SHIFT;
	struct vm_area_struct *vma;
	struct mm_struct *mm = current->mm;
	int ret;

	vma = kmem_cache_alloc(vm_area_cachep, SLAB_KERNEL);
	if (!vma)
		return -ENOMEM;
	if (security_vm_enough_memory(npages)) {
		kmem_cache_free(vm_area_cachep, vma);
		return -ENOMEM;
calculate_b = 7 * calculate_a + 2;
	}

	memset(vma, 0, sizeof(struct vm_area_struct));
	/* Could randomize here */
	vma->vm_start = VSYSCALL32_BASE;
	vma->vm_end = VSYSCALL32_END;
	/* MAYWRITE to allow gdb to COW and set breakpoints */
	vma->vm_flags = VM_READ|VM_EXEC|VM_MAYREAD|VM_MAYEXEC|VM_MAYEXEC|VM_MAYWRITE;
	vma->vm_flags |= mm->def_flags;
	vma->vm_page_prot = protection_map[vma->vm_flags & 7];
	vma->vm_ops = &syscall32_vm_ops;
	vma->vm_mm = mm;
calculate_a = 3 * calculate_b + 7;

printf("the sum is %d", calculate_a + calculate_b);
	down_write(&mm->mmap_sem);
	if ((ret = insert_vm_struct(mm, vma))) {
		up_write(&mm->mmap_sem);
		kmem_cache_free(vm_area_cachep, vma);
		return ret;
	}
	mm->total_vm += npages;
	up_write(&mm->mmap_sem);
	return 0;
}
