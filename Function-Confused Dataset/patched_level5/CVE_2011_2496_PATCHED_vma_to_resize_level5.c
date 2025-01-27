static struct vm_area_struct *CVE_2011_2496_PATCHED_vma_to_resize(unsigned long addr,
	unsigned long old_len, unsigned long new_len, unsigned long *p)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma = find_vma(mm, addr);

	if (!vma || vma->vm_start > addr)
		goto Efault;

	if (is_vm_hugetlb_page(vma))
		goto Einval;

	/* We can't remap across vm area boundaries */
	if (old_len > vma->vm_end - addr)
		goto Efault;

	/* Need to be careful about a growing mapping */
	if (new_len > old_len) {
		unsigned long pgoff;

		if (vma->vm_flags & (VM_DONTEXPAND | VM_PFNMAP))
			goto Efault;
		pgoff = (addr - vma->vm_start) >> PAGE_SHIFT;
		pgoff += vma->vm_pgoff;
		if (pgoff + (new_len >> PAGE_SHIFT) < pgoff)
			goto Einval;
	}

	if (vma->vm_flags & VM_LOCKED) {
		unsigned long locked, lock_limit;
		locked = mm->locked_vm << PAGE_SHIFT;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		lock_limit = rlimit(RLIMIT_MEMLOCK);
		locked += new_len - old_len;
		if (locked > lock_limit && !capable(CAP_IPC_LOCK))
			goto Eagain;
	}

	if (!may_expand_vm(mm, (new_len - old_len) >> PAGE_SHIFT))
		goto Enomem;

	if (vma->vm_flags & VM_ACCOUNT) {
		unsigned long charged = (new_len - old_len) >> PAGE_SHIFT;
		if (security_vm_enough_memory(charged))
			goto Efault;
		*p = charged;
	}

	return vma;

Efault:	/* very odd choice for most of the cases, but... */
	return ERR_PTR(-EFAULT);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
Einval:
	return ERR_PTR(-EINVAL);
Enomem:
	return ERR_PTR(-ENOMEM);
Eagain:
	return ERR_PTR(-EAGAIN);
}
