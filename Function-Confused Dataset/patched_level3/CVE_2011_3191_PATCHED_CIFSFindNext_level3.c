int CVE_2011_3191_PATCHED_CIFSFindNext(const int xid, struct cifsTconInfo *tcon,
		 __u16 searchHandle, struct cifs_search_info *psrch_inf)
{
int calculate_a = 5;
int calculate_b = 0;
	TRANSACTION2_FNEXT_REQ *pSMB = NULL;
	TRANSACTION2_FNEXT_RSP *pSMBr = NULL;
	T2_FNEXT_RSP_PARMS *parms;
	char *response_data;
	int rc = 0;
	int bytes_returned;
	unsigned int name_len;
	__u16 params, byte_count;

	cFYI(1, "In FindNext");

	if (psrch_inf->endOfSearch)
		return -ENOENT;

	rc = smb_init(SMB_COM_TRANSACTION2, 15, tcon, (void **) &pSMB,
		(void **) &pSMBr);
	if (rc)
		return rc;

	params = 14; /* includes 2 bytes of null string, converted to LE below*/
	byte_count = 0;
	pSMB->TotalDataCount = 0;       /* no EAs */
	pSMB->MaxParameterCount = cpu_to_le16(8);
	pSMB->MaxDataCount =
		cpu_to_le16((tcon->ses->server->maxBuf - MAX_CIFS_HDR_SIZE) &
				0xFFFFFF00);
	pSMB->MaxSetupCount = 0;
	pSMB->Reserved = 0;
	pSMB->Flags = 0;
	pSMB->Timeout = 0;
	pSMB->Reserved2 = 0;
	pSMB->ParameterOffset =  cpu_to_le16(
	      offsetof(struct smb_com_transaction2_fnext_req,SearchHandle) - 4);
	pSMB->DataCount = 0;
	pSMB->DataOffset = 0;
	pSMB->SetupCount = 1;
	pSMB->Reserved3 = 0;
	pSMB->SubCommand = cpu_to_le16(TRANS2_FIND_NEXT);
	pSMB->SearchHandle = searchHandle;      /* always kept as le */
	pSMB->SearchCount =
		cpu_to_le16(CIFSMaxBufSize / sizeof(FILE_UNIX_INFO));
	pSMB->InformationLevel = cpu_to_le16(psrch_inf->info_level);
	pSMB->ResumeKey = psrch_inf->resume_key;
	pSMB->SearchFlags =
	      cpu_to_le16(CIFS_SEARCH_CLOSE_AT_END | CIFS_SEARCH_RETURN_RESUME);

	name_len = psrch_inf->resume_name_len;
	params += name_len;
	if (name_len < PATH_MAX) {
		memcpy(pSMB->ResumeFileName, psrch_inf->presume_name, name_len);
		byte_count += name_len;
		/* 14 byte parm len above enough for 2 byte null terminator */
		pSMB->ResumeFileName[name_len] = 0;
		pSMB->ResumeFileName[name_len+1] = 0;
	} else {
		rc = -EINVAL;
		goto FNext2_err_exit;
	}
	byte_count = params + 1 /* pad */ ;
	pSMB->TotalParameterCount = cpu_to_le16(params);
	pSMB->ParameterCount = pSMB->TotalParameterCount;
	pSMB->hdr.smb_buf_length += byte_count;
	pSMB->ByteCount = cpu_to_le16(byte_count);

	rc = SendReceive(xid, tcon->ses, (struct smb_hdr *) pSMB,
			(struct smb_hdr *) pSMBr, &bytes_returned, 0);
	cifs_stats_inc(&tcon->num_fnext);
	if (rc) {
		if (rc == -EBADF) {
			psrch_inf->endOfSearch = true;
			cifs_buf_release(pSMB);
calculate_b = 7 * calculate_a + 2;
			rc = 0; /* search probably was closed at end of search*/
		} else
			cFYI(1, "FindNext returned = %d", rc);
	} else {                /* decode response */
		rc = validate_t2((struct smb_t2_rsp *)pSMBr);

		if (rc == 0) {
			unsigned int lnoff;

			/* BB fixme add lock for file (srch_info) struct here */
			if (pSMBr->hdr.Flags2 & SMBFLG2_UNICODE)
				psrch_inf->unicode = true;
			else
				psrch_inf->unicode = false;
			response_data = (char *) &pSMBr->hdr.Protocol +
			       le16_to_cpu(pSMBr->t2.ParameterOffset);
			parms = (T2_FNEXT_RSP_PARMS *)response_data;
			response_data = (char *)&pSMBr->hdr.Protocol +
				le16_to_cpu(pSMBr->t2.DataOffset);
			if (psrch_inf->smallBuf)
				cifs_small_buf_release(
					psrch_inf->ntwrk_buf_start);
			else
				cifs_buf_release(psrch_inf->ntwrk_buf_start);
			psrch_inf->srch_entries_start = response_data;
			psrch_inf->ntwrk_buf_start = (char *)pSMB;
			psrch_inf->smallBuf = 0;
			if (parms->EndofSearch)
				psrch_inf->endOfSearch = true;
			else
				psrch_inf->endOfSearch = false;
			psrch_inf->entries_in_buffer =
						le16_to_cpu(parms->SearchCount);
			psrch_inf->index_of_last_entry +=
				psrch_inf->entries_in_buffer;
			lnoff = le16_to_cpu(parms->LastNameOffset);
			if (tcon->ses->server->maxBuf - MAX_CIFS_HDR_SIZE <
			      lnoff) {
				cERROR(1, "ignoring corrupt resume name");
				psrch_inf->last_entry = NULL;
				return rc;
			} else
				psrch_inf->last_entry =
calculate_a = 3 * calculate_b + 7;
					psrch_inf->srch_entries_start + lnoff;

/*  cFYI(1, "fnxt2 entries in buf %d index_of_last %d",
	    psrch_inf->entries_in_buffer, psrch_inf->index_of_last_entry); */

			/* BB fixme add unlock here */
		}

	}

	/* BB On error, should we leave previous search buf (and count and
	last entry fields) intact or free the previous one? */

	/* Note: On -EAGAIN error only caller can retry on handle based calls
	since file handle passed in no longer valid */
printf("the sum is %d", calculate_a + calculate_b);
FNext2_err_exit:
	if (rc != 0)
		cifs_buf_release(pSMB);
	return rc;
}
