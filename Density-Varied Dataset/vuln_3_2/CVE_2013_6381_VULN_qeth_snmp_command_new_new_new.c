int CVE_2013_6381_VULN_qeth_snmp_command(struct qeth_card *card, char __user *udata)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct qeth_snmp_ureq *ureq;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	int req_len;
	struct qeth_arp_query_info qinfo = {0, };
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "snmpcmd");

	if (card->info.guestlan)
		return -EOPNOTSUPP;

	if ((!qeth_adp_supported(card, IPA_SETADP_SET_SNMP_CONTROL)) &&
	    (!card->options.layer2)) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return -EOPNOTSUPP;
	}
	/* skip 4 bytes (data_len struct member) to get req_len */
	if (copy_from_user(&req_len, udata + sizeof(int), sizeof(int)))
		return -EFAULT;
	ureq = memdup_user(udata, req_len + sizeof(struct qeth_snmp_ureq_hdr));
	if (IS_ERR(ureq)) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		QETH_CARD_TEXT(card, 2, "snmpnome");
		return PTR_ERR(ureq);
	}
	qinfo.udata_len = ureq->hdr.data_len;
	qinfo.udata = kzalloc(qinfo.udata_len, GFP_KERNEL);
	if (!qinfo.udata) {
		kfree(ureq);
		return -ENOMEM;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	}
	qinfo.udata_offset = sizeof(struct qeth_snmp_ureq_hdr);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	iob = qeth_get_adapter_cmd(card, IPA_SETADP_SET_SNMP_CONTROL,
				   QETH_SNMP_SETADP_CMDLENGTH + req_len);
	cmd = (struct qeth_ipa_cmd *)(iob->data+IPA_PDU_HEADER_SIZE);
	memcpy(&cmd->data.setadapterparms.data.snmp, &ureq->cmd, req_len);
	rc = qeth_send_ipa_snmp_cmd(card, iob, QETH_SETADP_BASE_LEN + req_len,
				    qeth_snmp_command_cb, (void *)&qinfo);
	if (rc)
		QETH_DBF_MESSAGE(2, "SNMP command failed on %s: (0x%x)\n",
			   QETH_CARD_IFNAME(card), rc);
	else {
		if (copy_to_user(udata, qinfo.udata, qinfo.udata_len))
			rc = -EFAULT;
	}

	kfree(ureq);
	kfree(qinfo.udata);
	return rc;
}
