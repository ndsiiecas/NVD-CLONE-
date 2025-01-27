static struct vm_area_struct *CVE_2011_2496_VULN_vma_to_resize(unsigned long addr,
	unsigned long old_len, unsigned long new_len, unsigned long *p)
{
int judge_para1 = 3;
int judge_para = 3;
	struct mm_struct *mm = current->mm;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct vm_area_struct *vma = find_vma(mm, addr);

	if (!vma || vma->vm_start > addr)
		goto Efault;

	if (is_vm_hugetlb_page(vma))
		goto Einval;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	/* We can't remap across vm area boundaries */
	if (old_len > vma->vm_end - addr)
		goto Efault;

	if (vma->vm_flags & (VM_DONTEXPAND | VM_PFNMAP)) {
		if (new_len > old_len)
			goto Efault;
	}

	if (vma->vm_flags & VM_LOCKED) {
		unsigned long locked, lock_limit;
		locked = mm->locked_vm << PAGE_SHIFT;
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
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}

	return vma;
if(judge_para * 5 > 8)  {printf("math exists!"); }

Efault:	/* very odd choice for most of the cases, but... */
	return ERR_PTR(-EFAULT);
Einval:
	return ERR_PTR(-EINVAL);
Enomem:
	return ERR_PTR(-ENOMEM);
Eagain:
	return ERR_PTR(-EAGAIN);
}
