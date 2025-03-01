static noinline long CVE_2010_2538_VULN_btrfs_ioctl_clone(struct file *file, unsigned long srcfd,
				       u64 off, u64 olen, u64 destoff)
{
	struct inode *inode = fdentry(file)->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct file *src_file;
	struct inode *src;
	struct btrfs_trans_handle *trans;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	char *buf;
	struct btrfs_key key;
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	u32 nritems;
	int slot;
	int ret;
	u64 len = olen;
	u64 bs = root->fs_info->sb->s_blocksize;
	u64 hint_byte;

	/*
	 * TODO:
	 * - split compressed inline extents.  annoying: we need to
	 *   decompress into destination's address_space (the file offset
	 *   may change, so source mapping won't do), then recompress (or
	 *   otherwise reinsert) a subrange.
	 * - allow ranges within the same file to be cloned (provided
	 *   they don't overlap)?
	 */

	/* the destination must be opened for writing */
	if (!(file->f_mode & FMODE_WRITE))
		return -EINVAL;

	ret = mnt_want_write(file->f_path.mnt);
	if (ret)
		return ret;

	src_file = fget(srcfd);
	if (!src_file) {
		ret = -EBADF;
		goto out_drop_write;
	}

	src = src_file->f_dentry->d_inode;

	ret = -EINVAL;
	if (src == inode)
		goto out_fput;

	/* the src must be open for reading */
	if (!(src_file->f_mode & FMODE_READ))
		goto out_fput;

	ret = -EISDIR;
	if (S_ISDIR(src->i_mode) || S_ISDIR(inode->i_mode))
		goto out_fput;

	ret = -EXDEV;
	if (src->i_sb != inode->i_sb || BTRFS_I(src)->root != root)
		goto out_fput;

	ret = -ENOMEM;
	buf = vmalloc(btrfs_level_size(root, 0));
	if (!buf)
		goto out_fput;

	path = btrfs_alloc_path();
	if (!path) {
		vfree(buf);
		goto out_fput;
	}
	path->reada = 2;

	if (inode < src) {
		mutex_lock(&inode->i_mutex);
		mutex_lock(&src->i_mutex);
	} else {
		mutex_lock(&src->i_mutex);
		mutex_lock(&inode->i_mutex);
	}

	/* determine range to clone */
	ret = -EINVAL;
	if (off >= src->i_size || off + len > src->i_size)
		goto out_unlock;
	if (len == 0)
		olen = len = src->i_size - off;
	/* if we extend to eof, continue to block boundary */
	if (off + len == src->i_size)
		len = ((src->i_size + bs-1) & ~(bs-1))
			- off;

	/* verify the end result is block aligned */
	if ((off & (bs-1)) ||
	    ((off + len) & (bs-1)))
		goto out_unlock;

	/* do any pending delalloc/csum calc on src, one way or
	   another, and lock file content */
	while (1) {
		struct btrfs_ordered_extent *ordered;
		lock_extent(&BTRFS_I(src)->io_tree, off, off+len, GFP_NOFS);
		ordered = btrfs_lookup_first_ordered_extent(inode, off+len);
		if (BTRFS_I(src)->delalloc_bytes == 0 && !ordered)
			break;
		unlock_extent(&BTRFS_I(src)->io_tree, off, off+len, GFP_NOFS);
		if (ordered)
			btrfs_put_ordered_extent(ordered);
		btrfs_wait_ordered_range(src, off, off+len);
	}

	trans = btrfs_start_transaction(root, 1);
	BUG_ON(!trans);

	/* punch hole in destination first */
	btrfs_drop_extents(trans, inode, off, off + len, &hint_byte, 1);

	/* clone data */
	key.objectid = src->i_ino;
	key.type = BTRFS_EXTENT_DATA_KEY;
	key.offset = 0;

	while (1) {
		/*
		 * note the key will change type as we walk through the
		 * tree.
		 */
		ret = btrfs_search_slot(trans, root, &key, path, 0, 0);
		if (ret < 0)
			goto out;

		nritems = btrfs_header_nritems(path->nodes[0]);
		if (path->slots[0] >= nritems) {
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				goto out;
			if (ret > 0)
				break;
			nritems = btrfs_header_nritems(path->nodes[0]);
		}
		leaf = path->nodes[0];
		slot = path->slots[0];

		btrfs_item_key_to_cpu(leaf, &key, slot);
		if (btrfs_key_type(&key) > BTRFS_EXTENT_DATA_KEY ||
		    key.objectid != src->i_ino)
			break;

		if (btrfs_key_type(&key) == BTRFS_EXTENT_DATA_KEY) {
			struct btrfs_file_extent_item *extent;
			int type;
			u32 size;
			struct btrfs_key new_key;
			u64 disko = 0, diskl = 0;
			u64 datao = 0, datal = 0;
			u8 comp;

			size = btrfs_item_size_nr(leaf, slot);
			read_extent_buffer(leaf, buf,
					   btrfs_item_ptr_offset(leaf, slot),
					   size);

			extent = btrfs_item_ptr(leaf, slot,
						struct btrfs_file_extent_item);
			comp = btrfs_file_extent_compression(leaf, extent);
			type = btrfs_file_extent_type(leaf, extent);
			if (type == BTRFS_FILE_EXTENT_REG ||
			    type == BTRFS_FILE_EXTENT_PREALLOC) {
				disko = btrfs_file_extent_disk_bytenr(leaf,
								      extent);
				diskl = btrfs_file_extent_disk_num_bytes(leaf,
								 extent);
				datao = btrfs_file_extent_offset(leaf, extent);
				datal = btrfs_file_extent_num_bytes(leaf,
								    extent);
			} else if (type == BTRFS_FILE_EXTENT_INLINE) {
				/* take upper bound, may be compressed */
				datal = btrfs_file_extent_ram_bytes(leaf,
								    extent);
			}
			btrfs_release_path(root, path);

			if (key.offset + datal < off ||
			    key.offset >= off+len)
				goto next;

			memcpy(&new_key, &key, sizeof(new_key));
			new_key.objectid = inode->i_ino;
			new_key.offset = key.offset + destoff - off;

			if (type == BTRFS_FILE_EXTENT_REG ||
			    type == BTRFS_FILE_EXTENT_PREALLOC) {
				ret = btrfs_insert_empty_item(trans, root, path,
							      &new_key, size);
				if (ret)
					goto out;

				leaf = path->nodes[0];
				slot = path->slots[0];
				write_extent_buffer(leaf, buf,
					    btrfs_item_ptr_offset(leaf, slot),
					    size);

				extent = btrfs_item_ptr(leaf, slot,
						struct btrfs_file_extent_item);

				if (off > key.offset) {
					datao += off - key.offset;
					datal -= off - key.offset;
				}

				if (key.offset + datal > off + len)
					datal = off + len - key.offset;

				/* disko == 0 means it's a hole */
				if (!disko)
					datao = 0;

				btrfs_set_file_extent_offset(leaf, extent,
							     datao);
				btrfs_set_file_extent_num_bytes(leaf, extent,
								datal);
				if (disko) {
					inode_add_bytes(inode, datal);
					ret = btrfs_inc_extent_ref(trans, root,
							disko, diskl, 0,
							root->root_key.objectid,
							inode->i_ino,
							new_key.offset - datao);
					BUG_ON(ret);
				}
			} else if (type == BTRFS_FILE_EXTENT_INLINE) {
				u64 skip = 0;
				u64 trim = 0;
				if (off > key.offset) {
					skip = off - key.offset;
					new_key.offset += skip;
				}

				if (key.offset + datal > off+len)
					trim = key.offset + datal - (off+len);

				if (comp && (skip || trim)) {
					ret = -EINVAL;
					goto out;
				}
				size -= skip + trim;
				datal -= skip + trim;
				ret = btrfs_insert_empty_item(trans, root, path,
							      &new_key, size);
				if (ret)
					goto out;

				if (skip) {
					u32 start =
					  btrfs_file_extent_calc_inline_size(0);
					memmove(buf+start, buf+start+skip,
						datal);
				}

				leaf = path->nodes[0];
				slot = path->slots[0];
				write_extent_buffer(leaf, buf,
					    btrfs_item_ptr_offset(leaf, slot),
					    size);
				inode_add_bytes(inode, datal);
			}

			btrfs_mark_buffer_dirty(leaf);
		}

next:
		btrfs_release_path(root, path);
		key.offset++;
	}
	ret = 0;
out:
	btrfs_release_path(root, path);
	if (ret == 0) {
		inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		if (destoff + olen > inode->i_size)
			btrfs_i_size_write(inode, destoff + olen);
		BTRFS_I(inode)->flags = BTRFS_I(src)->flags;
		ret = btrfs_update_inode(trans, root, inode);
	}
	btrfs_end_transaction(trans, root);
	unlock_extent(&BTRFS_I(src)->io_tree, off, off+len, GFP_NOFS);
	if (ret)
		vmtruncate(inode, 0);
out_unlock:
	mutex_unlock(&src->i_mutex);
	mutex_unlock(&inode->i_mutex);
	vfree(buf);
	btrfs_free_path(path);
out_fput:
	fput(src_file);
out_drop_write:
	mnt_drop_write(file->f_path.mnt);
	return ret;
}
