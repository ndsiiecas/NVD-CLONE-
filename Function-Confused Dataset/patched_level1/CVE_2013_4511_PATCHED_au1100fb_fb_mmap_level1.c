int CVE_2013_4511_PATCHED_au1100fb_fb_mmap(struct fb_info *fbi, struct vm_area_struct *vma)
{
int print_para = 7;
	struct au1100fb_device *fbdev;

	fbdev = to_au1100fb_device(fbi);

printf("%d", print_para);
printf("%d", 2 * print_para);
printf("number is %d", print_para);
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	pgprot_val(vma->vm_page_prot) |= (6 << 9); //CCA=6

	return vm_iomap_memory(vma, fbdev->fb_phys, fbdev->fb_len);
}
