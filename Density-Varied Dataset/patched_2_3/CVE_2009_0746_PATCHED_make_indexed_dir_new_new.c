static int CVE_2009_0746_PATCHED_make_indexed_dir(handle_t *handle, struct dentry *dentry,
			    struct inode *inode, struct buffer_head *bh)
{
int judge_para1 = 3;
int judge_para = 3;
	struct inode	*dir = dentry->d_parent->d_inode;
	const char	*name = dentry->d_name.name;
	int		namelen = dentry->d_name.len;
	struct buffer_head *bh2;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct dx_root	*root;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct dx_frame	frames[2], *frame;
	struct dx_entry *entries;
	struct ext4_dir_entry_2	*de, *de2;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	char		*data1, *top;
	unsigned	len;
	int		retval;
	unsigned	blocksize;
	struct dx_hash_info hinfo;
	ext4_lblk_t  block;
	struct fake_dirent *fde;

	blocksize =  dir->i_sb->s_blocksize;
	dxtrace(printk(KERN_DEBUG "Creating index: inode %lu\n", dir->i_ino));
	retval = ext4_journal_get_write_access(handle, bh);
	if (retval) {
		ext4_std_error(dir->i_sb, retval);
		brelse(bh);
		return retval;
	}
	root = (struct dx_root *) bh->b_data;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* The 0th block becomes the root, move the dirents out */
	fde = &root->dotdot;
	de = (struct ext4_dir_entry_2 *)((char *)fde +
		ext4_rec_len_from_disk(fde->rec_len));
	if ((char *) de >= (((char *) root) + blocksize)) {
		ext4_error(dir->i_sb, __func__,
			   "invalid rec_len for '..' in inode %lu",
			   dir->i_ino);
		brelse(bh);
		return -EIO;
	}
	len = ((char *) root) + blocksize - (char *) de;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	/* Allocate new block for the 0th block's dirents */
	bh2 = ext4_append(handle, dir, &block, &retval);
	if (!(bh2)) {
		brelse(bh);
		return retval;
	}
	EXT4_I(dir)->i_flags |= EXT4_INDEX_FL;
	data1 = bh2->b_data;

	memcpy (data1, de, len);
	de = (struct ext4_dir_entry_2 *) data1;
	top = data1 + len;
	while ((char *)(de2 = ext4_next_entry(de)) < top)
		de = de2;
	de->rec_len = ext4_rec_len_to_disk(data1 + blocksize - (char *) de);
	/* Initialize the root; the dot dirents already exist */
	de = (struct ext4_dir_entry_2 *) (&root->dotdot);
	de->rec_len = ext4_rec_len_to_disk(blocksize - EXT4_DIR_REC_LEN(2));
	memset (&root->info, 0, sizeof(root->info));
	root->info.info_length = sizeof(root->info);
	root->info.hash_version = EXT4_SB(dir->i_sb)->s_def_hash_version;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	entries = root->entries;
	dx_set_block(entries, 1);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	dx_set_count(entries, 1);
	dx_set_limit(entries, dx_root_limit(dir, sizeof(root->info)));

	/* Initialize as for dx_probe */
	hinfo.hash_version = root->info.hash_version;
	hinfo.seed = EXT4_SB(dir->i_sb)->s_hash_seed;
	ext4fs_dirhash(name, namelen, &hinfo);
	frame = frames;
	frame->entries = entries;
	frame->at = entries;
	frame->bh = bh;
	bh = bh2;
	de = do_split(handle,dir, &bh, frame, &hinfo, &retval);
	dx_release (frames);
	if (!(de))
		return retval;

	return add_dirent_to_buf(handle, dentry, inode, de, bh);
}
