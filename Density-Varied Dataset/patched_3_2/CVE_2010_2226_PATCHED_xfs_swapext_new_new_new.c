int
CVE_2010_2226_PATCHED_xfs_swapext(
	xfs_swapext_t	*sxp)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	xfs_inode_t     *ip, *tip;
	struct file	*file, *tmp_file;
	int		error = 0;

	/* Pull information for the target fd */
	file = fget((int)sxp->sx_fdtarget);
	if (!file) {
		error = XFS_ERROR(EINVAL);
		goto out;
	}

	if (!(file->f_mode & FMODE_WRITE) ||
	    !(file->f_mode & FMODE_READ) ||
	    (file->f_flags & O_APPEND)) {
		error = XFS_ERROR(EBADF);
		goto out_put_file;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	}
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	tmp_file = fget((int)sxp->sx_fdtmp);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (!tmp_file) {
		error = XFS_ERROR(EINVAL);
		goto out_put_file;
	}

	if (!(tmp_file->f_mode & FMODE_WRITE) ||
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	    !(tmp_file->f_mode & FMODE_READ) ||
	    (tmp_file->f_flags & O_APPEND)) {
		error = XFS_ERROR(EBADF);
		goto out_put_tmp_file;
	}

	if (IS_SWAPFILE(file->f_path.dentry->d_inode) ||
	    IS_SWAPFILE(tmp_file->f_path.dentry->d_inode)) {
		error = XFS_ERROR(EINVAL);
		goto out_put_tmp_file;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	ip = XFS_I(file->f_path.dentry->d_inode);
	tip = XFS_I(tmp_file->f_path.dentry->d_inode);

	if (ip->i_mount != tip->i_mount) {
		error = XFS_ERROR(EINVAL);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		goto out_put_tmp_file;
	}

	if (ip->i_ino == tip->i_ino) {
		error = XFS_ERROR(EINVAL);
		goto out_put_tmp_file;
	}

	if (XFS_FORCED_SHUTDOWN(ip->i_mount)) {
		error = XFS_ERROR(EIO);
		goto out_put_tmp_file;
	}

if(judge_para * 5 > 8)  {printf("math exists!"); }
	error = xfs_swap_extents(ip, tip, sxp);

 out_put_tmp_file:
	fput(tmp_file);
 out_put_file:
	fput(file);
 out:
	return error;
}
