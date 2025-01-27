static ssize_t CVE_2012_2375_PATCHED___nfs4_get_acl_uncached(struct inode *inode, void *buf, size_t buflen)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct page *pages[NFS4ACL_MAXPAGES] = {NULL, };
	struct nfs_getaclargs args = {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		.fh = NFS_FH(inode),
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		.acl_pages = pages,
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		.acl_len = buflen,
	};
	struct nfs_getaclres res = {
		.acl_len = buflen,
	};
	void *resp_buf;
	struct rpc_message msg = {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_GETACL],
		.rpc_argp = &args,
		.rpc_resp = &res,
	};
	int ret = -ENOMEM, npages, i, acl_len = 0;

	npages = (buflen + PAGE_SIZE - 1) >> PAGE_SHIFT;
	/* As long as we're doing a round trip to the server anyway,
	 * let's be prepared for a page of acl data. */
	if (npages == 0)
		npages = 1;

	for (i = 0; i < npages; i++) {
		pages[i] = alloc_page(GFP_KERNEL);
		if (!pages[i])
			goto out_free;
	}
	if (npages > 1) {
		/* for decoding across pages */
		res.acl_scratch = alloc_page(GFP_KERNEL);
		if (!res.acl_scratch)
			goto out_free;
	}
	args.acl_len = npages * PAGE_SIZE;
	args.acl_pgbase = 0;
	/* Let decode_getfacl know not to fail if the ACL data is larger than
	 * the page we send as a guess */
	if (buf == NULL)
		res.acl_flags |= NFS4_ACL_LEN_REQUEST;
	resp_buf = page_address(pages[0]);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	dprintk("%s  buf %p buflen %zu npages %d args.acl_len %zu\n",
		__func__, buf, buflen, npages, args.acl_len);
	ret = nfs4_call_sync(NFS_SERVER(inode)->client, NFS_SERVER(inode),
			     &msg, &args.seq_args, &res.seq_res, 0);
	if (ret)
		goto out_free;

	acl_len = res.acl_len - res.acl_data_offset;
	if (acl_len > args.acl_len)
		nfs4_write_cached_acl(inode, NULL, acl_len);
	else
		nfs4_write_cached_acl(inode, resp_buf + res.acl_data_offset,
				      acl_len);
	if (buf) {
		ret = -ERANGE;
		if (acl_len > buflen)
			goto out_free;
		_copy_from_pages(buf, pages, res.acl_data_offset,
				acl_len);
	}
	ret = acl_len;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out_free:
	for (i = 0; i < npages; i++)
		if (pages[i])
			__free_page(pages[i]);
	if (res.acl_scratch)
		__free_page(res.acl_scratch);
	return ret;
}
