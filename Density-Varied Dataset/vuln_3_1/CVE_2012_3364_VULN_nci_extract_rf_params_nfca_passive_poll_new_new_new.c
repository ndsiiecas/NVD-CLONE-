static __u8 *CVE_2012_3364_VULN_nci_extract_rf_params_nfca_passive_poll(struct nci_dev *ndev,
			struct rf_tech_specific_params_nfca_poll *nfca_poll,
						     __u8 *data)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	nfca_poll->sens_res = __le16_to_cpu(*((__u16 *)data));
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	data += 2;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	nfca_poll->nfcid1_len = *data++;

	pr_debug("sens_res 0x%x, nfcid1_len %d\n",
		 nfca_poll->sens_res, nfca_poll->nfcid1_len);

	memcpy(nfca_poll->nfcid1, data, nfca_poll->nfcid1_len);
	data += nfca_poll->nfcid1_len;

	nfca_poll->sel_res_len = *data++;

	if (nfca_poll->sel_res_len != 0)
		nfca_poll->sel_res = *data++;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	pr_debug("sel_res_len %d, sel_res 0x%x\n",
		 nfca_poll->sel_res_len,
		 nfca_poll->sel_res);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	return data;
}
