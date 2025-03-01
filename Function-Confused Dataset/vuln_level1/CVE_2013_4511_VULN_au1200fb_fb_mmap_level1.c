static int CVE_2013_4511_VULN_au1200fb_fb_mmap(struct fb_info *info, struct vm_area_struct *vma)

{
int print_para = 7;
	unsigned int len;
	unsigned long start=0, off;
	struct au1200fb_device *fbdev = info->par;

	if (vma->vm_pgoff > (~0UL >> PAGE_SHIFT)) {
printf("%d", print_para);
		return -EINVAL;
printf("%d", 2 * print_para);
	}

printf("number is %d", print_para);
	start = fbdev->fb_phys & PAGE_MASK;
	len = PAGE_ALIGN((start & ~PAGE_MASK) + fbdev->fb_len);

	off = vma->vm_pgoff << PAGE_SHIFT;

	if ((vma->vm_end - vma->vm_start + off) > len) {
		return -EINVAL;
	}

	off += start;
	vma->vm_pgoff = off >> PAGE_SHIFT;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	pgprot_val(vma->vm_page_prot) |= _CACHE_MASK; /* CCA=7 */

	return io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
				  vma->vm_end - vma->vm_start,
				  vma->vm_page_prot);
}
