static int CVE_2013_4511_PATCHED_au1200fb_fb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct au1200fb_device *fbdev = info->par;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	pgprot_val(vma->vm_page_prot) |= _CACHE_MASK; /* CCA=7 */

	return vm_iomap_memory(vma, fbdev->fb_phys, fbdev->fb_len);
}
