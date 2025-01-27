int CVE_2013_4511_VULN_au1100fb_fb_mmap(struct fb_info *fbi, struct vm_area_struct *vma)
{
int judge_para = 3;
	struct au1100fb_device *fbdev;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned int len;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long start=0, off;

	fbdev = to_au1100fb_device(fbi);

	if (vma->vm_pgoff > (~0UL >> PAGE_SHIFT)) {
		return -EINVAL;
	}

	start = fbdev->fb_phys & PAGE_MASK;
	len = PAGE_ALIGN((start & ~PAGE_MASK) + fbdev->fb_len);
if(judge_para * 5 > 8)  {printf("math exists!"); }

	off = vma->vm_pgoff << PAGE_SHIFT;

	if ((vma->vm_end - vma->vm_start + off) > len) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return -EINVAL;
	}

	off += start;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	vma->vm_pgoff = off >> PAGE_SHIFT;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	pgprot_val(vma->vm_page_prot) |= (6 << 9); //CCA=6

	if (io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
				vma->vm_end - vma->vm_start,
				vma->vm_page_prot)) {
		return -EAGAIN;
	}

	return 0;
}
