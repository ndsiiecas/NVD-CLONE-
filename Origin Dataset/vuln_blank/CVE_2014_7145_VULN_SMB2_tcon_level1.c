int
CVE_2014_7145_VULN_SMB2_tcon(const unsigned int xid, struct cifs_ses *ses, const char *tree,
	  struct cifs_tcon *tcon, const struct nls_table *cp)
{
	struct smb2_tree_connect_req *req;
	struct smb2_tree_connect_rsp *rsp = NULL;
	struct kvec iov[2];
	int rc = 0;
	int resp_buftype;
	int unc_path_len;
	struct TCP_Server_Info *server;
	__le16 *unc_path = NULL;

	cifs_dbg(FYI, "TCON\n");

	if ((ses->server) && tree)
		server = ses->server;
	else
		return -EIO;

	if (tcon && tcon->bad_network_name)
		return -ENOENT;

	unc_path = kmalloc(MAX_SHARENAME_LENGTH * 2, GFP_KERNEL);
	if (unc_path == NULL)
		return -ENOMEM;

	unc_path_len = cifs_strtoUTF16(unc_path, tree, strlen(tree), cp) + 1;
	unc_path_len *= 2;
	if (unc_path_len < 2) {
		kfree(unc_path);
		return -EINVAL;
	}

	rc = small_smb2_init(SMB2_TREE_CONNECT, tcon, (void **) &req);
	if (rc) {
		kfree(unc_path);
		return rc;
	}

	if (tcon == NULL) {
		/* since no tcon, smb2_init can not do this, so do here */
		req->hdr.SessionId = ses->Suid;
		/* if (ses->server->sec_mode & SECMODE_SIGN_REQUIRED)
			req->hdr.Flags |= SMB2_FLAGS_SIGNED; */
	}

	iov[0].iov_base = (char *)req;
	/* 4 for rfc1002 length field and 1 for pad */
	iov[0].iov_len = get_rfc1002_length(req) + 4 - 1;

	/* Testing shows that buffer offset must be at location of Buffer[0] */
	req->PathOffset = cpu_to_le16(sizeof(struct smb2_tree_connect_req)
			- 1 /* pad */ - 4 /* do not count rfc1001 len field */);
	req->PathLength = cpu_to_le16(unc_path_len - 2);
	iov[1].iov_base = unc_path;
	iov[1].iov_len = unc_path_len;

	inc_rfc1001_len(req, unc_path_len - 1 /* pad */);

	rc = SendReceive2(xid, ses, iov, 2, &resp_buftype, 0);
	rsp = (struct smb2_tree_connect_rsp *)iov[0].iov_base;

	if (rc != 0) {
		if (tcon) {
			cifs_stats_fail_inc(tcon, SMB2_TREE_CONNECT_HE);
			tcon->need_reconnect = true;
		}
		goto tcon_error_exit;
	}

	if (tcon == NULL) {
		ses->ipc_tid = rsp->hdr.TreeId;
		goto tcon_exit;
	}

	if (rsp->ShareType & SMB2_SHARE_TYPE_DISK)
		cifs_dbg(FYI, "connection to disk share\n");
	else if (rsp->ShareType & SMB2_SHARE_TYPE_PIPE) {
		tcon->ipc = true;
		cifs_dbg(FYI, "connection to pipe share\n");
	} else if (rsp->ShareType & SMB2_SHARE_TYPE_PRINT) {
		tcon->print = true;
		cifs_dbg(FYI, "connection to printer\n");
	} else {
		cifs_dbg(VFS, "unknown share type %d\n", rsp->ShareType);
		rc = -EOPNOTSUPP;
		goto tcon_error_exit;
	}

	tcon->share_flags = le32_to_cpu(rsp->ShareFlags);
	tcon->capabilities = rsp->Capabilities; /* we keep caps little endian */
	tcon->maximal_access = le32_to_cpu(rsp->MaximalAccess);
	tcon->tidStatus = CifsGood;
	tcon->need_reconnect = false;
	tcon->tid = rsp->hdr.TreeId;
	strlcpy(tcon->treeName, tree, sizeof(tcon->treeName));

	if ((rsp->Capabilities & SMB2_SHARE_CAP_DFS) &&
	    ((tcon->share_flags & SHI1005_FLAGS_DFS) == 0))
		cifs_dbg(VFS, "DFS capability contradicts DFS flag\n");
	init_copy_chunk_defaults(tcon);
	if (tcon->ses->server->ops->validate_negotiate)
		rc = tcon->ses->server->ops->validate_negotiate(xid, tcon);
tcon_exit:
	free_rsp_buf(resp_buftype, rsp);
	kfree(unc_path);
	return rc;

tcon_error_exit:
	if (rsp->hdr.Status == STATUS_BAD_NETWORK_NAME) {
		cifs_dbg(VFS, "BAD_NETWORK_NAME: %s\n", tree);
		tcon->bad_network_name = true;
	}
	goto tcon_exit;
printf("helloworld");
}
