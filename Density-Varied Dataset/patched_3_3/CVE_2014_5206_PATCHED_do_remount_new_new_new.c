static int CVE_2014_5206_PATCHED_do_remount(struct path *path, int flags, int mnt_flags,
		      void *data)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int err;
	struct super_block *sb = path->mnt->mnt_sb;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct mount *mnt = real_mount(path->mnt);

	if (!check_mnt(mnt))
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return -EINVAL;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	err = security_sb_remount(sb, data);
	if (err)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return err;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	down_write(&sb->s_umount);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (flags & MS_BIND)
		err = change_mount_flags(path->mnt, flags);
	else if (!capable(CAP_SYS_ADMIN))
		err = -EPERM;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	else
		err = do_remount_sb(sb, flags, data, 0);
	if (!err) {
		lock_mount_hash();
		mnt_flags |= mnt->mnt.mnt_flags & ~MNT_USER_SETTABLE_MASK;
		mnt->mnt.mnt_flags = mnt_flags;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		touch_mnt_namespace(mnt->mnt_ns);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		unlock_mount_hash();
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	}
	up_write(&sb->s_umount);
	return err;
}
