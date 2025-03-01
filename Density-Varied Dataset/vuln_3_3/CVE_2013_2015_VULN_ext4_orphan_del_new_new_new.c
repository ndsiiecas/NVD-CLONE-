 * CVE_2013_2015_VULN_ext4_orphan_del() removes an unlinked or truncated inode from the list
 * of such inodes stored on disk, because it is finally being cleaned up.
 */
int CVE_2013_2015_VULN_ext4_orphan_del(handle_t *handle, struct inode *inode)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct list_head *prev;
	struct ext4_inode_info *ei = EXT4_I(inode);
	struct ext4_sb_info *sbi;
	__u32 ino_next;
	struct ext4_iloc iloc;
	int err = 0;

	if (!EXT4_SB(inode->i_sb)->s_journal)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return 0;

	mutex_lock(&EXT4_SB(inode->i_sb)->s_orphan_lock);
	if (list_empty(&ei->i_orphan))
		goto out;

	ino_next = NEXT_ORPHAN(inode);
	prev = ei->i_orphan.prev;
	sbi = EXT4_SB(inode->i_sb);

	jbd_debug(4, "remove inode %lu from orphan list\n", inode->i_ino);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	list_del_init(&ei->i_orphan);

	/* If we're on an error path, we may not have a valid
	 * transaction handle with which to update the orphan list on
	 * disk, but we still need to remove the inode from the linked
	 * list in memory. */
	if (!handle)
		goto out;

	err = ext4_reserve_inode_write(handle, inode, &iloc);
	if (err)
		goto out_err;

	if (prev == &sbi->s_orphan) {
		jbd_debug(4, "superblock will point to %u\n", ino_next);
		BUFFER_TRACE(sbi->s_sbh, "get_write_access");
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		err = ext4_journal_get_write_access(handle, sbi->s_sbh);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (err)
			goto out_brelse;
		sbi->s_es->s_last_orphan = cpu_to_le32(ino_next);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		err = ext4_handle_dirty_super(handle, inode->i_sb);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	} else {
		struct ext4_iloc iloc2;
		struct inode *i_prev =
			&list_entry(prev, struct ext4_inode_info, i_orphan)->vfs_inode;

		jbd_debug(4, "orphan inode %lu will point to %u\n",
			  i_prev->i_ino, ino_next);
		err = ext4_reserve_inode_write(handle, i_prev, &iloc2);
		if (err)
			goto out_brelse;
		NEXT_ORPHAN(i_prev) = ino_next;
		err = ext4_mark_iloc_dirty(handle, i_prev, &iloc2);
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (err)
		goto out_brelse;
	NEXT_ORPHAN(inode) = 0;
	err = ext4_mark_iloc_dirty(handle, inode, &iloc);

out_err:
	ext4_std_error(inode->i_sb, err);
out:
	mutex_unlock(&EXT4_SB(inode->i_sb)->s_orphan_lock);
	return err;
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out_brelse:
	brelse(iloc.bh);
	goto out_err;
}
