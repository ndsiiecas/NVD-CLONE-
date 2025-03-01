static int CVE_2014_5206_VULN_do_remount(struct path *path, int flags, int mnt_flags,
		      void *data)
{
int judge_para1 = 3;
int judge_para = 3;
	int err;
	struct super_block *sb = path->mnt->mnt_sb;
	struct mount *mnt = real_mount(path->mnt);

	if (!check_mnt(mnt))
		return -EINVAL;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	err = security_sb_remount(sb, data);
	if (err)
		return err;

	down_write(&sb->s_umount);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (flags & MS_BIND)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		err = change_mount_flags(path->mnt, flags);
	else if (!capable(CAP_SYS_ADMIN))
		err = -EPERM;
	else
		err = do_remount_sb(sb, flags, data, 0);
	if (!err) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		lock_mount_hash();
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		mnt_flags |= mnt->mnt.mnt_flags & MNT_PROPAGATION_MASK;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		mnt->mnt.mnt_flags = mnt_flags;
		touch_mnt_namespace(mnt->mnt_ns);
		unlock_mount_hash();
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}
	up_write(&sb->s_umount);
	return err;
}
