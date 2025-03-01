int CVE_2010_3015_PATCHED_ext4_ext_get_blocks(handle_t *handle, struct inode *inode,
			ext4_lblk_t iblock,
			unsigned int max_blocks, struct buffer_head *bh_result,
			int flags)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct ext4_ext_path *path = NULL;
	struct ext4_extent_header *eh;
	struct ext4_extent newex, *ex;
	ext4_fsblk_t newblock;
	int err = 0, depth, ret, cache_type;
	unsigned int allocated = 0;
	struct ext4_allocation_request ar;
	ext4_io_end_t *io = EXT4_I(inode)->cur_aio_dio;

	__clear_bit(BH_New, &bh_result->b_state);
	ext_debug("blocks %u/%u requested for inode %lu\n",
			iblock, max_blocks, inode->i_ino);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* check in cache */
	cache_type = ext4_ext_in_cache(inode, iblock, &newex);
	if (cache_type) {
		if (cache_type == EXT4_EXT_CACHE_GAP) {
			if ((flags & EXT4_GET_BLOCKS_CREATE) == 0) {
				/*
				 * block isn't allocated yet and
				 * user doesn't want to allocate it
				 */
				goto out2;
			}
			/* we should allocate requested block */
		} else if (cache_type == EXT4_EXT_CACHE_EXTENT) {
			/* block is already allocated */
			newblock = iblock
				   - le32_to_cpu(newex.ee_block)
				   + ext_pblock(&newex);
			/* number of remaining blocks in the extent */
			allocated = ext4_ext_get_actual_len(&newex) -
					(iblock - le32_to_cpu(newex.ee_block));
			goto out;
		} else {
			BUG();
		}
	}

	/* find extent for this block */
	path = ext4_ext_find_extent(inode, iblock, NULL);
	if (IS_ERR(path)) {
		err = PTR_ERR(path);
		path = NULL;
		goto out2;
	}

	depth = ext_depth(inode);

	/*
	 * consistent leaf must not be empty;
	 * this situation is possible, though, _during_ tree modification;
	 * this is why assert can't be put in ext4_ext_find_extent()
	 */
	if (path[depth].p_ext == NULL && depth != 0) {
		ext4_error(inode->i_sb, __func__, "bad extent address "
			   "inode: %lu, iblock: %d, depth: %d",
			   inode->i_ino, iblock, depth);
		err = -EIO;
		goto out2;
	}
	eh = path[depth].p_hdr;

	ex = path[depth].p_ext;
	if (ex) {
		ext4_lblk_t ee_block = le32_to_cpu(ex->ee_block);
		ext4_fsblk_t ee_start = ext_pblock(ex);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		unsigned short ee_len;

		/*
		 * Uninitialized extents are treated as holes, except that
		 * we split out initialized portions during a write.
		 */
		ee_len = ext4_ext_get_actual_len(ex);
		/* if found extent covers block, simply return it */
		if (in_range(iblock, ee_block, ee_len)) {
			newblock = iblock - ee_block + ee_start;
			/* number of remaining blocks in the extent */
			allocated = ee_len - (iblock - ee_block);
			ext_debug("%u fit into %u:%d -> %llu\n", iblock,
					ee_block, ee_len, newblock);

			/* Do not put uninitialized extent in the cache */
			if (!ext4_ext_is_uninitialized(ex)) {
				ext4_ext_put_in_cache(inode, ee_block,
							ee_len, ee_start,
							EXT4_EXT_CACHE_EXTENT);
				goto out;
			}
			ret = ext4_ext_handle_uninitialized_extents(handle,
					inode, iblock, max_blocks, path,
					flags, allocated, bh_result, newblock);
			return ret;
		}
	}

	/*
	 * requested block isn't allocated yet;
	 * we couldn't try to create block if create flag is zero
	 */
	if ((flags & EXT4_GET_BLOCKS_CREATE) == 0) {
		/*
		 * put just found gap into cache to speed up
		 * subsequent requests
		 */
		ext4_ext_put_gap_in_cache(inode, path, iblock);
		goto out2;
	}
	/*
	 * Okay, we need to do block allocation.
	 */

	/* find neighbour allocated blocks */
	ar.lleft = iblock;
	err = ext4_ext_search_left(inode, path, &ar.lleft, &ar.pleft);
	if (err)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		goto out2;
	ar.lright = iblock;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	err = ext4_ext_search_right(inode, path, &ar.lright, &ar.pright);
	if (err)
		goto out2;

	/*
	 * See if request is beyond maximum number of blocks we can have in
	 * a single extent. For an initialized extent this limit is
	 * EXT_INIT_MAX_LEN and for an uninitialized extent this limit is
	 * EXT_UNINIT_MAX_LEN.
	 */
	if (max_blocks > EXT_INIT_MAX_LEN &&
	    !(flags & EXT4_GET_BLOCKS_UNINIT_EXT))
		max_blocks = EXT_INIT_MAX_LEN;
	else if (max_blocks > EXT_UNINIT_MAX_LEN &&
		 (flags & EXT4_GET_BLOCKS_UNINIT_EXT))
		max_blocks = EXT_UNINIT_MAX_LEN;

	/* Check if we can really insert (iblock)::(iblock+max_blocks) extent */
	newex.ee_block = cpu_to_le32(iblock);
	newex.ee_len = cpu_to_le16(max_blocks);
	err = ext4_ext_check_overlap(inode, &newex, path);
	if (err)
		allocated = ext4_ext_get_actual_len(&newex);
	else
		allocated = max_blocks;

	/* allocate new block */
	ar.inode = inode;
	ar.goal = ext4_ext_find_goal(inode, path, iblock);
	ar.logical = iblock;
	ar.len = allocated;
	if (S_ISREG(inode->i_mode))
		ar.flags = EXT4_MB_HINT_DATA;
	else
		/* disable in-core preallocation for non-regular files */
		ar.flags = 0;
	newblock = ext4_mb_new_blocks(handle, &ar, &err);
	if (!newblock)
		goto out2;
	ext_debug("allocate new block: goal %llu, found %llu/%u\n",
		  ar.goal, newblock, allocated);

	/* try to insert new extent into found leaf and return */
	ext4_ext_store_pblock(&newex, newblock);
	newex.ee_len = cpu_to_le16(ar.len);
	/* Mark uninitialized */
	if (flags & EXT4_GET_BLOCKS_UNINIT_EXT){
		ext4_ext_mark_uninitialized(&newex);
		/*
		 * io_end structure was created for every async
		 * direct IO write to the middle of the file.
		 * To avoid unecessary convertion for every aio dio rewrite
		 * to the mid of file, here we flag the IO that is really
		 * need the convertion.
		 * For non asycn direct IO case, flag the inode state
		 * that we need to perform convertion when IO is done.
		 */
		if (flags == EXT4_GET_BLOCKS_DIO_CREATE_EXT) {
			if (io)
				io->flag = DIO_AIO_UNWRITTEN;
			else
				EXT4_I(inode)->i_state |=
					EXT4_STATE_DIO_UNWRITTEN;;
		}
	}
	err = ext4_ext_insert_extent(handle, inode, path, &newex, flags);
	if (err) {
		/* free data blocks we just allocated */
		/* not a good idea to call discard here directly,
		 * but otherwise we'd need to call it every free() */
		ext4_discard_preallocations(inode);
		ext4_free_blocks(handle, inode, 0, ext_pblock(&newex),
				 ext4_ext_get_actual_len(&newex), 0);
		goto out2;
	}

	/* previous routine could use block we allocated */
	newblock = ext_pblock(&newex);
	allocated = ext4_ext_get_actual_len(&newex);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (allocated > max_blocks)
		allocated = max_blocks;
	set_buffer_new(bh_result);

	/*
	 * Update reserved blocks/metadata blocks after successful
	 * block allocation which had been deferred till now.
	 */
	if (flags & EXT4_GET_BLOCKS_DELALLOC_RESERVE)
		ext4_da_update_reserve_space(inode, allocated, 1);

	/*
	 * Cache the extent and update transaction to commit on fdatasync only
	 * when it is _not_ an uninitialized extent.
	 */
	if ((flags & EXT4_GET_BLOCKS_UNINIT_EXT) == 0) {
		ext4_ext_put_in_cache(inode, iblock, allocated, newblock,
						EXT4_EXT_CACHE_EXTENT);
		ext4_update_inode_fsync_trans(handle, inode, 1);
	} else
		ext4_update_inode_fsync_trans(handle, inode, 0);
out:
	if (allocated > max_blocks)
		allocated = max_blocks;
	ext4_ext_show_leaf(inode, path);
	set_buffer_mapped(bh_result);
	bh_result->b_bdev = inode->i_sb->s_bdev;
	bh_result->b_blocknr = newblock;
out2:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (path) {
		ext4_ext_drop_refs(path);
		kfree(path);
	}
	return err ? err : allocated;
}
