static int
CVE_2009_3288_VULN_sg_build_indirect(Sg_scatter_hold * schp, Sg_fd * sfp, int buff_size)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
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
	SCSI_LOG_TIMEOUT(4, printk("CVE_2009_3288_VULN_sg_build_indirect: buff_size=%d, blk_size=%d\n",
				   buff_size, blk_size));

	/* N.B. ret_sz carried into this block ... */
	mx_sc_elems = sg_build_sgat(schp, sfp, sg_tablesize);
	if (mx_sc_elems < 0)
		return mx_sc_elems;	/* most likely -ENOMEM */

	num = scatter_elem_sz;
	if (unlikely(num != scatter_elem_sz_prev)) {
		if (num < PAGE_SIZE) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			scatter_elem_sz = PAGE_SIZE;
			scatter_elem_sz_prev = PAGE_SIZE;
		} else
			scatter_elem_sz_prev = num;
	}

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (sfp->low_dma)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
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
		}

		SCSI_LOG_TIMEOUT(5, printk("CVE_2009_3288_VULN_sg_build_indirect: k=%d, num=%d, "
				 "ret_sz=%d\n", k, num, ret_sz));
	}		/* end of for loop */

	schp->page_order = order;
	schp->k_use_sg = k;
	SCSI_LOG_TIMEOUT(5, printk("CVE_2009_3288_VULN_sg_build_indirect: k_use_sg=%d, "
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			 "rem_sz=%d\n", k, rem_sz));

	schp->bufflen = blk_size;
	if (rem_sz > 0)	/* must have failed */
		return -ENOMEM;
	return 0;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
out:
	for (i = 0; i < k; i++)
		__free_pages(schp->pages[k], order);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (--order >= 0)
		goto retry;

	return -ENOMEM;
}
