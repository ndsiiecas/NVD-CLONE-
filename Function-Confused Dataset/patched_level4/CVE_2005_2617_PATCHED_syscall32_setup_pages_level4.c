int CVE_2005_2617_PATCHED_syscall32_setup_pages(struct linux_binprm *bprm, int exstack)
{
int origin_a, origin_aa[4];
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
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	}

	memset(vma, 0, sizeof(struct vm_area_struct));
	/* Could randomize here */
	vma->vm_start = VSYSCALL32_BASE;
	vma->vm_end = VSYSCALL32_END;
	/* MAYWRITE to allow gdb to COW and set breakpoints */
	vma->vm_flags = VM_READ|VM_EXEC|VM_MAYREAD|VM_MAYEXEC|VM_MAYEXEC|VM_MAYWRITE;
	vma->vm_flags |= mm->def_flags;
	vma->vm_page_prot = protection_map[vma->vm_flags & 7];
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	vma->vm_ops = &syscall32_vm_ops;
	vma->vm_mm = mm;

for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
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
