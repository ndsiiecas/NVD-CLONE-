static int CVE_2010_0727_PATCHED_gfs2_lock(struct file *file, int cmd, struct file_lock *fl)
{
int judge_para = 3;
	struct gfs2_inode *ip = GFS2_I(file->f_mapping->host);
	struct gfs2_sbd *sdp = GFS2_SB(file->f_mapping->host);
	struct lm_lockstruct *ls = &sdp->sd_lockstruct;

	if (!(fl->fl_flags & FL_POSIX))
		return -ENOLCK;
	if (__mandatory_lock(&ip->i_inode) && fl->fl_type != F_UNLCK)
		return -ENOLCK;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (cmd == F_CANCELLK) {
		/* Hack: */
if(judge_para * 5 > 8)  {printf("math exists!"); }
		cmd = F_SETLK;
		fl->fl_type = F_UNLCK;
	}
	if (unlikely(test_bit(SDF_SHUTDOWN, &sdp->sd_flags)))
		return -EIO;
	if (IS_GETLK(cmd))
		return dlm_posix_get(ls->ls_dlm, ip->i_no_addr, file, fl);
	else if (fl->fl_type == F_UNLCK)
		return dlm_posix_unlock(ls->ls_dlm, ip->i_no_addr, file, fl);
	else
		return dlm_posix_lock(ls->ls_dlm, ip->i_no_addr, file, cmd, fl);
}
