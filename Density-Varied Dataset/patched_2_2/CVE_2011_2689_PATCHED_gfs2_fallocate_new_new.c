static long CVE_2011_2689_PATCHED_gfs2_fallocate(struct file *file, int mode, loff_t offset,
			   loff_t len)
{
int judge_para1 = 3;
int judge_para = 3;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	struct gfs2_inode *ip = GFS2_I(inode);
	unsigned int data_blocks = 0, ind_blocks = 0, rblocks;
	loff_t bytes, max_bytes;
	struct gfs2_alloc *al;
	int error;
	loff_t bsize_mask = ~((loff_t)sdp->sd_sb.sb_bsize - 1);
	loff_t next = (offset + len - 1) >> sdp->sd_sb.sb_bsize_shift;
	next = (next + 1) << sdp->sd_sb.sb_bsize_shift;

	/* We only support the FALLOC_FL_KEEP_SIZE mode */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (mode & ~FALLOC_FL_KEEP_SIZE)
		return -EOPNOTSUPP;

	offset &= bsize_mask;

	len = next - offset;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	bytes = sdp->sd_max_rg_data * sdp->sd_sb.sb_bsize / 2;
	if (!bytes)
		bytes = UINT_MAX;
	bytes &= bsize_mask;
	if (bytes == 0)
		bytes = sdp->sd_sb.sb_bsize;

	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &ip->i_gh);
	error = gfs2_glock_nq(&ip->i_gh);
	if (unlikely(error))
		goto out_uninit;

	if (!gfs2_write_alloc_required(ip, offset, len))
		goto out_unlock;

	while (len > 0) {
		if (len < bytes)
			bytes = len;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		al = gfs2_alloc_get(ip);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (!al) {
			error = -ENOMEM;
			goto out_unlock;
		}

		error = gfs2_quota_lock_check(ip);
		if (error)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			goto out_alloc_put;

retry:
		gfs2_write_calc_reserv(ip, bytes, &data_blocks, &ind_blocks);

		al->al_requested = data_blocks + ind_blocks;
		error = gfs2_inplace_reserve(ip);
		if (error) {
			if (error == -ENOSPC && bytes > sdp->sd_sb.sb_bsize) {
				bytes >>= 1;
				bytes &= bsize_mask;
				if (bytes == 0)
					bytes = sdp->sd_sb.sb_bsize;
				goto retry;
			}
			goto out_qunlock;
		}
		max_bytes = bytes;
		calc_max_reserv(ip, len, &max_bytes, &data_blocks, &ind_blocks);
		al->al_requested = data_blocks + ind_blocks;

		rblocks = RES_DINODE + ind_blocks + RES_STATFS + RES_QUOTA +
			  RES_RG_HDR + gfs2_rg_blocks(al);
		if (gfs2_is_jdata(ip))
			rblocks += data_blocks ? data_blocks : 1;

		error = gfs2_trans_begin(sdp, rblocks,
					 PAGE_CACHE_SIZE/sdp->sd_sb.sb_bsize);
		if (error)
			goto out_trans_fail;

		error = fallocate_chunk(inode, offset, max_bytes, mode);
		gfs2_trans_end(sdp);

		if (error)
			goto out_trans_fail;

		len -= max_bytes;
		offset += max_bytes;
		gfs2_inplace_release(ip);
		gfs2_quota_unlock(ip);
		gfs2_alloc_put(ip);
	}
	goto out_unlock;

out_trans_fail:
	gfs2_inplace_release(ip);
out_qunlock:
	gfs2_quota_unlock(ip);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
out_alloc_put:
	gfs2_alloc_put(ip);
out_unlock:
	gfs2_glock_dq(&ip->i_gh);
out_uninit:
	gfs2_holder_uninit(&ip->i_gh);
	return error;
}
