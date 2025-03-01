unsigned long CVE_2009_3888_PATCHED_do_mmap_pgoff(struct file *file,
			    unsigned long addr,
			    unsigned long len,
			    unsigned long prot,
			    unsigned long flags,
			    unsigned long pgoff)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct vm_area_struct *vma;
	struct vm_region *region;
	struct rb_node *rb;
	unsigned long capabilities, vm_flags, result;
	int ret;

	kenter(",%lx,%lx,%lx,%lx,%lx", addr, len, prot, flags, pgoff);

	if (!(flags & MAP_FIXED))
		addr = round_hint_to_min(addr);

	/* decide whether we should attempt the mapping, and if so what sort of
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 * mapping */
	ret = validate_mmap_request(file, addr, len, prot, flags, pgoff,
				    &capabilities);
	if (ret < 0) {
		kleave(" = %d [val]", ret);
		return ret;
	}

	/* we've determined that we can make the mapping, now translate what we
	 * now know into VMA flags */
	vm_flags = determine_vm_flags(file, prot, flags, capabilities);

	/* we're going to need to record the mapping */
	region = kmem_cache_zalloc(vm_region_jar, GFP_KERNEL);
	if (!region)
		goto error_getting_region;

	vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL);
	if (!vma)
		goto error_getting_vma;

	atomic_set(&region->vm_usage, 1);
	region->vm_flags = vm_flags;
	region->vm_pgoff = pgoff;

	INIT_LIST_HEAD(&vma->anon_vma_node);
	vma->vm_flags = vm_flags;
	vma->vm_pgoff = pgoff;

	if (file) {
		region->vm_file = file;
		get_file(file);
		vma->vm_file = file;
		get_file(file);
		if (vm_flags & VM_EXECUTABLE) {
			added_exe_file_vma(current->mm);
			vma->vm_mm = current->mm;
		}
	}

	down_write(&nommu_region_sem);

	/* if we want to share, we need to check for regions created by other
	 * mmap() calls that overlap with our proposed mapping
	 * - we can only share with a superset match on most regular files
	 * - shared mappings on character devices and memory backed files are
	 *   permitted to overlap inexactly as far as we are concerned for in
	 *   these cases, sharing is handled in the driver or filesystem rather
	 *   than here
	 */
	if (vm_flags & VM_MAYSHARE) {
		struct vm_region *pregion;
		unsigned long pglen, rpglen, pgend, rpgend, start;

		pglen = (len + PAGE_SIZE - 1) >> PAGE_SHIFT;
		pgend = pgoff + pglen;

		for (rb = rb_first(&nommu_region_tree); rb; rb = rb_next(rb)) {
			pregion = rb_entry(rb, struct vm_region, vm_rb);

			if (!(pregion->vm_flags & VM_MAYSHARE))
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				continue;

			/* search for overlapping mappings on the same file */
			if (pregion->vm_file->f_path.dentry->d_inode !=
			    file->f_path.dentry->d_inode)
				continue;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			if (pregion->vm_pgoff >= pgend)
				continue;

			rpglen = pregion->vm_end - pregion->vm_start;
			rpglen = (rpglen + PAGE_SIZE - 1) >> PAGE_SHIFT;
			rpgend = pregion->vm_pgoff + rpglen;
			if (pgoff >= rpgend)
				continue;

			/* handle inexactly overlapping matches between
			 * mappings */
			if ((pregion->vm_pgoff != pgoff || rpglen != pglen) &&
			    !(pgoff >= pregion->vm_pgoff && pgend <= rpgend)) {
				/* new mapping is not a subset of the region */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				if (!(capabilities & BDI_CAP_MAP_DIRECT))
					goto sharing_violation;
				continue;
			}

			/* we've found a region we can share */
			atomic_inc(&pregion->vm_usage);
			vma->vm_region = pregion;
			start = pregion->vm_start;
			start += (pgoff - pregion->vm_pgoff) << PAGE_SHIFT;
			vma->vm_start = start;
			vma->vm_end = start + len;

			if (pregion->vm_flags & VM_MAPPED_COPY) {
				kdebug("share copy");
				vma->vm_flags |= VM_MAPPED_COPY;
			} else {
				kdebug("share mmap");
				ret = do_mmap_shared_file(vma);
				if (ret < 0) {
					vma->vm_region = NULL;
					vma->vm_start = 0;
					vma->vm_end = 0;
					atomic_dec(&pregion->vm_usage);
					pregion = NULL;
					goto error_just_free;
				}
			}
			fput(region->vm_file);
			kmem_cache_free(vm_region_jar, region);
			region = pregion;
			result = start;
if(judge_para * 5 > 8)  {printf("math exists!"); }
			goto share;
		}

		/* obtain the address at which to make a shared mapping
		 * - this is the hook for quasi-memory character devices to
		 *   tell us the location of a shared mapping
		 */
		if (file && file->f_op->get_unmapped_area) {
			addr = file->f_op->get_unmapped_area(file, addr, len,
							     pgoff, flags);
			if (IS_ERR((void *) addr)) {
				ret = addr;
				if (ret != (unsigned long) -ENOSYS)
					goto error_just_free;

				/* the driver refused to tell us where to site
				 * the mapping so we'll have to attempt to copy
				 * it */
				ret = (unsigned long) -ENODEV;
				if (!(capabilities & BDI_CAP_MAP_COPY))
					goto error_just_free;

				capabilities &= ~BDI_CAP_MAP_DIRECT;
			} else {
				vma->vm_start = region->vm_start = addr;
				vma->vm_end = region->vm_end = addr + len;
			}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		}
	}

	vma->vm_region = region;
	add_nommu_region(region);

	/* set up the mapping */
	if (file && vma->vm_flags & VM_SHARED)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		ret = do_mmap_shared_file(vma);
	else
		ret = do_mmap_private(vma, region, len);
	if (ret < 0)
		goto error_put_region;

	/* okay... we have a mapping; now we have to register it */
	result = vma->vm_start;

	current->mm->total_vm += len >> PAGE_SHIFT;

share:
	add_vma_to_mm(current->mm, vma);

	up_write(&nommu_region_sem);

	if (prot & PROT_EXEC)
		flush_icache_range(result, result + len);

	kleave(" = %lx", result);
	return result;

error_put_region:
	__put_nommu_region(region);
	if (vma) {
		if (vma->vm_file) {
			fput(vma->vm_file);
			if (vma->vm_flags & VM_EXECUTABLE)
				removed_exe_file_vma(vma->vm_mm);
		}
		kmem_cache_free(vm_area_cachep, vma);
	}
	kleave(" = %d [pr]", ret);
	return ret;

error_just_free:
	up_write(&nommu_region_sem);
error:
	if (region->vm_file)
		fput(region->vm_file);
	kmem_cache_free(vm_region_jar, region);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (vma->vm_file)
		fput(vma->vm_file);
	if (vma->vm_flags & VM_EXECUTABLE)
		removed_exe_file_vma(vma->vm_mm);
	kmem_cache_free(vm_area_cachep, vma);
	kleave(" = %d", ret);
	return ret;

sharing_violation:
	up_write(&nommu_region_sem);
	printk(KERN_WARNING "Attempt to share mismatched mappings\n");
	ret = -EINVAL;
	goto error;

error_getting_vma:
	kmem_cache_free(vm_region_jar, region);
	printk(KERN_WARNING "Allocation of vma for %lu byte allocation"
	       " from process %d failed\n",
	       len, current->pid);
	show_free_areas();
	return -ENOMEM;

error_getting_region:
	printk(KERN_WARNING "Allocation of vm region for %lu byte allocation"
	       " from process %d failed\n",
	       len, current->pid);
	show_free_areas();
	return -ENOMEM;
}
