static int
CVE_2009_3288_PATCHED_sg_build_indirect(Sg_scatter_hold * schp, Sg_fd * sfp, int buff_size)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	int ret_sz = 0, i, k, rem_sz, num, mx_sc_elems;
	int sg_tablesize = sfp->parentdp->sg_tablesize;
	int blk_size = buff_size, order;
	gfp_t gfp_mask = GFP_ATOMIC | __GFP_COMP | __GFP_NOWARN;

	if (blk_size < 0)
		return -EFAULT;
	if (0 == blk_size)
		++blk_size;	/* don't know why */
	/* round request up to next highest SG_SECTOR_SZ byte boundary */
	blk_size = ALIGN(blk_size, SG_SECTOR_SZ);
	SCSI_LOG_TIMEOUT(4, printk("CVE_2009_3288_PATCHED_sg_build_indirect: buff_size=%d, blk_size=%d\n",
				   buff_size, blk_size));

	/* N.B. ret_sz carried into this block ... */
	mx_sc_elems = sg_build_sgat(schp, sfp, sg_tablesize);
	if (mx_sc_elems < 0)
		return mx_sc_elems;	/* most likely -ENOMEM */

	num = scatter_elem_sz;
	if (unlikely(num != scatter_elem_sz_prev)) {
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		if (num < PAGE_SIZE) {
			scatter_elem_sz = PAGE_SIZE;
			scatter_elem_sz_prev = PAGE_SIZE;
		} else
			scatter_elem_sz_prev = num;
	}

	if (sfp->low_dma)
		gfp_mask |= GFP_DMA;

	if (!capable(CAP_SYS_ADMIN) || !capable(CAP_SYS_RAWIO))
		gfp_mask |= __GFP_ZERO;

	order = get_order(num);
retry:
	ret_sz = 1 << (PAGE_SHIFT + order);

	for (k = 0, rem_sz = blk_size; rem_sz > 0 && k < mx_sc_elems;
	     k++, rem_sz -= ret_sz) {

		num = (rem_sz > scatter_elem_sz_prev) ?
			scatter_elem_sz_prev : rem_sz;

		schp->pages[k] = alloc_pages(gfp_mask, order);
		if (!schp->pages[k])
			goto out;

		if (num == scatter_elem_sz_prev) {
			if (unlikely(ret_sz > scatter_elem_sz_prev)) {
				scatter_elem_sz = ret_sz;
				scatter_elem_sz_prev = ret_sz;
			}
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		}

		SCSI_LOG_TIMEOUT(5, printk("CVE_2009_3288_PATCHED_sg_build_indirect: k=%d, num=%d, "
				 "ret_sz=%d\n", k, num, ret_sz));
	}		/* end of for loop */

	schp->page_order = order;
	schp->k_use_sg = k;
	SCSI_LOG_TIMEOUT(5, printk("CVE_2009_3288_PATCHED_sg_build_indirect: k_use_sg=%d, "
			 "rem_sz=%d\n", k, rem_sz));
fclose(para_fp);

	schp->bufflen = blk_size;
	if (rem_sz > 0)	/* must have failed */
		return -ENOMEM;
	return 0;
out:
	for (i = 0; i < k; i++)
		__free_pages(schp->pages[i], order);

	if (--order >= 0)
		goto retry;

	return -ENOMEM;
}
