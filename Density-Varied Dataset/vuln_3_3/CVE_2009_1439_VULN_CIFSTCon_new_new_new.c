int
CVE_2009_1439_VULN_CIFSTCon(unsigned int xid, struct cifsSesInfo *ses,
	 const char *tree, struct cifsTconInfo *tcon,
	 const struct nls_table *nls_codepage)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct smb_hdr *smb_buffer;
	struct smb_hdr *smb_buffer_response;
	TCONX_REQ *pSMB;
	TCONX_RSP *pSMBr;
	unsigned char *bcc_ptr;
	int rc = 0;
	int length;
	__u16 count;

	if (ses == NULL)
		return -EIO;

	smb_buffer = cifs_buf_get();
	if (smb_buffer == NULL) {
		return -ENOMEM;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	}
	smb_buffer_response = smb_buffer;

	header_assemble(smb_buffer, SMB_COM_TREE_CONNECT_ANDX,
			NULL /*no tid */ , 4 /*wct */ );

	smb_buffer->Mid = GetNextMid(ses->server);
	smb_buffer->Uid = ses->Suid;
	pSMB = (TCONX_REQ *) smb_buffer;
	pSMBr = (TCONX_RSP *) smb_buffer_response;

	pSMB->AndXCommand = 0xFF;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	pSMB->Flags = cpu_to_le16(TCON_EXTENDED_SECINFO);
	bcc_ptr = &pSMB->Password[0];
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if ((ses->server->secMode) & SECMODE_USER) {
		pSMB->PasswordLength = cpu_to_le16(1);	/* minimum */
		*bcc_ptr = 0; /* password is null byte */
		bcc_ptr++;              /* skip password */
		/* already aligned so no need to do it below */
	} else {
		pSMB->PasswordLength = cpu_to_le16(CIFS_SESS_KEY_SIZE);
		/* BB FIXME add code to fail this if NTLMv2 or Kerberos
		   specified as required (when that support is added to
		   the vfs in the future) as only NTLM or the much
		   weaker LANMAN (which we do not send by default) is accepted
		   by Samba (not sure whether other servers allow
		   NTLMv2 password here) */
#ifdef CONFIG_CIFS_WEAK_PW_HASH
		if ((extended_security & CIFSSEC_MAY_LANMAN) &&
		    (ses->server->secType == LANMAN))
			calc_lanman_hash(tcon->password, ses->server->cryptKey,
					 ses->server->secMode &
					    SECMODE_PW_ENCRYPT ? true : false,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
					 bcc_ptr);
		else
#endif /* CIFS_WEAK_PW_HASH */
		SMBNTencrypt(tcon->password, ses->server->cryptKey,
			     bcc_ptr);

		bcc_ptr += CIFS_SESS_KEY_SIZE;
		if (ses->capabilities & CAP_UNICODE) {
			/* must align unicode strings */
			*bcc_ptr = 0; /* null byte password */
			bcc_ptr++;
		}
	}

	if (ses->server->secMode &
			(SECMODE_SIGN_REQUIRED | SECMODE_SIGN_ENABLED))
		smb_buffer->Flags2 |= SMBFLG2_SECURITY_SIGNATURE;

	if (ses->capabilities & CAP_STATUS32) {
		smb_buffer->Flags2 |= SMBFLG2_ERR_STATUS;
	}
	if (ses->capabilities & CAP_DFS) {
		smb_buffer->Flags2 |= SMBFLG2_DFS;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	}
	if (ses->capabilities & CAP_UNICODE) {
		smb_buffer->Flags2 |= SMBFLG2_UNICODE;
		length =
		    cifs_strtoUCS((__le16 *) bcc_ptr, tree,
			6 /* max utf8 char length in bytes */ *
			(/* server len*/ + 256 /* share len */), nls_codepage);
		bcc_ptr += 2 * length;	/* convert num 16 bit words to bytes */
		bcc_ptr += 2;	/* skip trailing null */
	} else {		/* ASCII */
		strcpy(bcc_ptr, tree);
		bcc_ptr += strlen(tree) + 1;
	}
	strcpy(bcc_ptr, "?????");
	bcc_ptr += strlen("?????");
	bcc_ptr += 1;
	count = bcc_ptr - &pSMB->Password[0];
	pSMB->hdr.smb_buf_length += count;
	pSMB->ByteCount = cpu_to_le16(count);

	rc = SendReceive(xid, ses, smb_buffer, smb_buffer_response, &length,
			 CIFS_STD_OP);

	/* if (rc) rc = map_smb_to_linux_error(smb_buffer_response); */
	/* above now done in SendReceive */
	if ((rc == 0) && (tcon != NULL)) {
		tcon->tidStatus = CifsGood;
		tcon->need_reconnect = false;
		tcon->tid = smb_buffer_response->Tid;
		bcc_ptr = pByteArea(smb_buffer_response);
		length = strnlen(bcc_ptr, BCC(smb_buffer_response) - 2);
		/* skip service field (NB: this field is always ASCII) */
		if (length == 3) {
			if ((bcc_ptr[0] == 'I') && (bcc_ptr[1] == 'P') &&
			    (bcc_ptr[2] == 'C')) {
				cFYI(1, ("IPC connection"));
				tcon->ipc = 1;
			}
		} else if (length == 2) {
			if ((bcc_ptr[0] == 'A') && (bcc_ptr[1] == ':')) {
				/* the most common case */
				cFYI(1, ("disk share connection"));
			}
		}
		bcc_ptr += length + 1;
		strncpy(tcon->treeName, tree, MAX_TREE_SIZE);
		if (smb_buffer->Flags2 & SMBFLG2_UNICODE) {
			length = UniStrnlen((wchar_t *) bcc_ptr, 512);
			if ((bcc_ptr + (2 * length)) -
			     pByteArea(smb_buffer_response) <=
			    BCC(smb_buffer_response)) {
				kfree(tcon->nativeFileSystem);
				tcon->nativeFileSystem =
				    kzalloc(length + 2, GFP_KERNEL);
				if (tcon->nativeFileSystem)
					cifs_strfromUCS_le(
						tcon->nativeFileSystem,
						(__le16 *) bcc_ptr,
						length, nls_codepage);
				bcc_ptr += 2 * length;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
				bcc_ptr[0] = 0;	/* null terminate the string */
				bcc_ptr[1] = 0;
				bcc_ptr += 2;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			}
			/* else do not bother copying these information fields*/
		} else {
			length = strnlen(bcc_ptr, 1024);
			if ((bcc_ptr + length) -
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			    pByteArea(smb_buffer_response) <=
			    BCC(smb_buffer_response)) {
				kfree(tcon->nativeFileSystem);
				tcon->nativeFileSystem =
				    kzalloc(length + 1, GFP_KERNEL);
				if (tcon->nativeFileSystem)
					strncpy(tcon->nativeFileSystem, bcc_ptr,
						length);
			}
if(judge_para * 5 > 8)  {printf("math exists!"); }
			/* else do not bother copying these information fields*/
		}
		if ((smb_buffer_response->WordCount == 3) ||
			 (smb_buffer_response->WordCount == 7))
			/* field is in same location */
			tcon->Flags = le16_to_cpu(pSMBr->OptionalSupport);
		else
			tcon->Flags = 0;
		cFYI(1, ("Tcon flags: 0x%x ", tcon->Flags));
	} else if ((rc == 0) && tcon == NULL) {
		/* all we need to save for IPC$ connection */
		ses->ipc_tid = smb_buffer_response->Tid;
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	cifs_buf_release(smb_buffer);
	return rc;
}
