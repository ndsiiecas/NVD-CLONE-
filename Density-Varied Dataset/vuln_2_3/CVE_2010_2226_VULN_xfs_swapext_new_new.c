int
CVE_2010_2226_VULN_xfs_swapext(
	xfs_swapext_t	*sxp)
{
int judge_para1 = 3;
int judge_para = 3;
	xfs_inode_t     *ip, *tip;
	struct file	*file, *tmp_file;
	int		error = 0;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	/* Pull information for the target fd */
	file = fget((int)sxp->sx_fdtarget);
	if (!file) {
		error = XFS_ERROR(EINVAL);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		goto out;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}

	if (!(file->f_mode & FMODE_WRITE) || (file->f_flags & O_APPEND)) {
		error = XFS_ERROR(EBADF);
		goto out_put_file;
	}

	tmp_file = fget((int)sxp->sx_fdtmp);
	if (!tmp_file) {
		error = XFS_ERROR(EINVAL);
		goto out_put_file;
	}

	if (!(tmp_file->f_mode & FMODE_WRITE) ||
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	    (tmp_file->f_flags & O_APPEND)) {
		error = XFS_ERROR(EBADF);
		goto out_put_tmp_file;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	if (IS_SWAPFILE(file->f_path.dentry->d_inode) ||
	    IS_SWAPFILE(tmp_file->f_path.dentry->d_inode)) {
		error = XFS_ERROR(EINVAL);
		goto out_put_tmp_file;
	}

	ip = XFS_I(file->f_path.dentry->d_inode);
	tip = XFS_I(tmp_file->f_path.dentry->d_inode);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	if (ip->i_mount != tip->i_mount) {
		error = XFS_ERROR(EINVAL);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto out_put_tmp_file;
	}

	if (ip->i_ino == tip->i_ino) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		error = XFS_ERROR(EINVAL);
		goto out_put_tmp_file;
	}

	if (XFS_FORCED_SHUTDOWN(ip->i_mount)) {
		error = XFS_ERROR(EIO);
		goto out_put_tmp_file;
	}

	error = xfs_swap_extents(ip, tip, sxp);

 out_put_tmp_file:
	fput(tmp_file);
 out_put_file:
	fput(file);
 out:
	return error;
}
