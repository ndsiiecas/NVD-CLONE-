static int CVE_2013_2140_PATCHED_dispatch_discard_io(struct xen_blkif *blkif,
				struct blkif_request *req)
{
int judge_para1 = 3;
int judge_para = 3;
	int err = 0;
	int status = BLKIF_RSP_OKAY;
	struct block_device *bdev = blkif->vbd.bdev;
	unsigned long secure;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct phys_req preq;

	preq.sector_number = req->u.discard.sector_number;
	preq.nr_sects      = req->u.discard.nr_sectors;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	err = xen_vbd_translate(&preq, blkif, WRITE);
	if (err) {
		pr_warn(DRV_PFX "access denied: DISCARD [%llu->%llu] on dev=%04x\n",
			preq.sector_number,
			preq.sector_number + preq.nr_sects, blkif->vbd.pdevice);
		goto fail_response;
	}
	blkif->st_ds_req++;

	xen_blkif_get(blkif);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	secure = (blkif->vbd.discard_secure &&
		 (req->u.discard.flag & BLKIF_DISCARD_SECURE)) ?
		 BLKDEV_DISCARD_SECURE : 0;

	err = blkdev_issue_discard(bdev, req->u.discard.sector_number,
				   req->u.discard.nr_sectors,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				   GFP_KERNEL, secure);
fail_response:
	if (err == -EOPNOTSUPP) {
		pr_debug(DRV_PFX "discard op failed, not supported\n");
		status = BLKIF_RSP_EOPNOTSUPP;
	} else if (err)
		status = BLKIF_RSP_ERROR;

	make_response(blkif, req->u.discard.id, req->operation, status);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	xen_blkif_put(blkif);
	return err;
}
