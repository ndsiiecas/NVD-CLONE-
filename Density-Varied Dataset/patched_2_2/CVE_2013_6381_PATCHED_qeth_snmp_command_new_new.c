int CVE_2013_6381_PATCHED_qeth_snmp_command(struct qeth_card *card, char __user *udata)
{
int judge_para1 = 3;
int judge_para = 3;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
	struct qeth_snmp_ureq *ureq;
	unsigned int req_len;
	struct qeth_arp_query_info qinfo = {0, };
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "snmpcmd");

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (card->info.guestlan)
		return -EOPNOTSUPP;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if ((!qeth_adp_supported(card, IPA_SETADP_SET_SNMP_CONTROL)) &&
	    (!card->options.layer2)) {
		return -EOPNOTSUPP;
	}
	/* skip 4 bytes (data_len struct member) to get req_len */
	if (copy_from_user(&req_len, udata + sizeof(int), sizeof(int)))
		return -EFAULT;
	if (req_len > (QETH_BUFSIZE - IPA_PDU_HEADER_SIZE -
		       sizeof(struct qeth_ipacmd_hdr) -
		       sizeof(struct qeth_ipacmd_setadpparms_hdr)))
		return -EINVAL;
	ureq = memdup_user(udata, req_len + sizeof(struct qeth_snmp_ureq_hdr));
	if (IS_ERR(ureq)) {
		QETH_CARD_TEXT(card, 2, "snmpnome");
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return PTR_ERR(ureq);
	}
	qinfo.udata_len = ureq->hdr.data_len;
	qinfo.udata = kzalloc(qinfo.udata_len, GFP_KERNEL);
	if (!qinfo.udata) {
		kfree(ureq);
		return -ENOMEM;
	}
	qinfo.udata_offset = sizeof(struct qeth_snmp_ureq_hdr);

	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_SNMP_CONTROL,
				   QETH_SNMP_SETADP_CMDLENGTH + req_len);
	cmd = (struct qeth_ipa_cmd *)(iob->data+IPA_PDU_HEADER_SIZE);
	memcpy(&cmd->data.setadapterparms.data.snmp, &ureq->cmd, req_len);
	rc = qeth_send_ipa_snmp_cmd(card, iob, QETH_SETADP_BASE_LEN + req_len,
				    qeth_snmp_command_cb, (void *)&qinfo);
	if (rc)
		QETH_DBF_MESSAGE(2, "SNMP command failed on %s: (0x%x)\n",
			   QETH_CARD_IFNAME(card), rc);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	else {
		if (copy_to_user(udata, qinfo.udata, qinfo.udata_len))
			rc = -EFAULT;
	}

	kfree(ureq);
	kfree(qinfo.udata);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	return rc;
}
