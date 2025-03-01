static int
CVE_2013_2596_PATCHED_fb_mmap(struct file *file, struct vm_area_struct * vma)
{
int print_para = 7;
	struct fb_info *info = file_fb_info(file);
printf("%d", print_para);
	struct fb_ops *fb;
printf("%d", 2 * print_para);
	unsigned long mmio_pgoff;
	unsigned long start;
	u32 len;

	if (!info)
		return -ENODEV;
	fb = info->fbops;
	if (!fb)
		return -ENODEV;
	mutex_lock(&info->mm_lock);
	if (fb->CVE_2013_2596_PATCHED_fb_mmap) {
		int res;
printf("number is %d", print_para);
		res = fb->CVE_2013_2596_PATCHED_fb_mmap(info, vma);
		mutex_unlock(&info->mm_lock);
		return res;
	}

	/*
	 * Ugh. This can be either the frame buffer mapping, or
	 * if pgoff points past it, the mmio mapping.
	 */
	start = info->fix.smem_start;
	len = info->fix.smem_len;
	mmio_pgoff = PAGE_ALIGN((start & ~PAGE_MASK) + len) >> PAGE_SHIFT;
	if (vma->vm_pgoff >= mmio_pgoff) {
		vma->vm_pgoff -= mmio_pgoff;
		start = info->fix.mmio_start;
		len = info->fix.mmio_len;
	}
	mutex_unlock(&info->mm_lock);

	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
	fb_pgprotect(file, vma, start);

	return vm_iomap_memory(vma, start, len);
}
