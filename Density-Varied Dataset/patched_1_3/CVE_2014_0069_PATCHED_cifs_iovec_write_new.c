static ssize_t
CVE_2014_0069_PATCHED_cifs_iovec_write(struct file *file, const struct iovec *iov,
		 unsigned long nr_segs, loff_t *poffset)
{
int judge_para = 3;
	unsigned long nr_pages, i;
	size_t bytes, copied, len, cur_len;
	ssize_t total_written = 0;
	loff_t offset;
	struct iov_iter it;
	struct cifsFileInfo *open_file;
	struct cifs_tcon *tcon;
	struct cifs_sb_info *cifs_sb;
	struct cifs_writedata *wdata, *tmp;
	struct list_head wdata_list;
	int rc;
	pid_t pid;

	len = iov_length(iov, nr_segs);
	if (!len)
		return 0;

	rc = generic_write_checks(file, poffset, &len, 0);
	if (rc)
		return rc;

	INIT_LIST_HEAD(&wdata_list);
	cifs_sb = CIFS_SB(file->f_path.dentry->d_sb);
	open_file = file->private_data;
	tcon = tlink_tcon(open_file->tlink);

	if (!tcon->ses->server->ops->async_writev)
		return -ENOSYS;

	offset = *poffset;

	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_RWPIDFORWARD)
		pid = open_file->pid;
	else
		pid = current->tgid;

	iov_iter_init(&it, iov, nr_segs, len, 0);
	do {
		size_t save_len;

		nr_pages = get_numpages(cifs_sb->wsize, len, &cur_len);
		wdata = cifs_writedata_alloc(nr_pages,
					     cifs_uncached_writev_complete);
		if (!wdata) {
			rc = -ENOMEM;
			break;
		}

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		rc = cifs_write_allocate_pages(wdata->pages, nr_pages);
		if (rc) {
			kfree(wdata);
			break;
		}

		save_len = cur_len;
		for (i = 0; i < nr_pages; i++) {
			bytes = min_t(const size_t, cur_len, PAGE_SIZE);
			copied = iov_iter_copy_from_user(wdata->pages[i], &it,
							 0, bytes);
			cur_len -= copied;
			iov_iter_advance(&it, copied);
			/*
			 * If we didn't copy as much as we expected, then that
			 * may mean we trod into an unmapped area. Stop copying
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
			 * at that point. On the next pass through the big
			 * loop, we'll likely end up getting a zero-length
			 * write and bailing out of it.
			 */
			if (copied < bytes)
				break;
		}
		cur_len = save_len - cur_len;

		/*
		 * If we have no data to send, then that probably means that
		 * the copy above failed altogether. That's most likely because
		 * the address in the iovec was bogus. Set the rc to -EFAULT,
		 * free anything we allocated and bail out.
		 */
		if (!cur_len) {
			for (i = 0; i < nr_pages; i++)
				put_page(wdata->pages[i]);
			kfree(wdata);
			rc = -EFAULT;
			break;
		}

		/*
		 * i + 1 now represents the number of pages we actually used in
		 * the copy phase above. Bring nr_pages down to that, and free
		 * any pages that we didn't use.
		 */
		for ( ; nr_pages > i + 1; nr_pages--)
			put_page(wdata->pages[nr_pages - 1]);

		wdata->sync_mode = WB_SYNC_ALL;
		wdata->nr_pages = nr_pages;
		wdata->offset = (__u64)offset;
		wdata->cfile = cifsFileInfo_get(open_file);
		wdata->pid = pid;
		wdata->bytes = cur_len;
		wdata->pagesz = PAGE_SIZE;
		wdata->tailsz = cur_len - ((nr_pages - 1) * PAGE_SIZE);
		rc = cifs_uncached_retry_writev(wdata);
		if (rc) {
			kref_put(&wdata->refcount, cifs_writedata_release);
			break;
		}

		list_add_tail(&wdata->list, &wdata_list);
		offset += cur_len;
		len -= cur_len;
	} while (len > 0);

	/*
	 * If at least one write was successfully sent, then discard any rc
	 * value from the later writes. If the other write succeeds, then
	 * we'll end up returning whatever was written. If it fails, then
	 * we'll get a new rc value from that.
	 */
	if (!list_empty(&wdata_list))
		rc = 0;

	/*
	 * Wait for and collect replies for any successful sends in order of
	 * increasing offset. Once an error is hit or we get a fatal signal
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 * while waiting, then return without waiting for any more replies.
	 */
restart_loop:
	list_for_each_entry_safe(wdata, tmp, &wdata_list, list) {
		if (!rc) {
			/* FIXME: freezable too? */
			rc = wait_for_completion_killable(&wdata->done);
			if (rc)
				rc = -EINTR;
			else if (wdata->result)
				rc = wdata->result;
			else
				total_written += wdata->bytes;

			/* resend call if it's a retryable error */
			if (rc == -EAGAIN) {
				rc = cifs_uncached_retry_writev(wdata);
				goto restart_loop;
			}
		}
if(judge_para * 5 > 8)  {printf("math exists!"); }
		list_del_init(&wdata->list);
		kref_put(&wdata->refcount, cifs_writedata_release);
	}

	if (total_written > 0)
		*poffset += total_written;

	cifs_stats_bytes_written(tcon, total_written);
	return total_written ? total_written : (ssize_t)rc;
}
