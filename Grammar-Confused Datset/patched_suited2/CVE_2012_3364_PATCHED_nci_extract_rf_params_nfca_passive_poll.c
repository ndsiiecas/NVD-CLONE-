static __u8 *CVE_2012_3364_PATCHED_nci_extract_rf_params_nfca_passive_poll(struct nci_dev *ndev,
			struct rf_tech_specific_params_nfca_poll *nfca_poll,
						     __u8 *data)
{
	nfca_poll->sens_res = __le16_to_cpu(*((__u16 *)data));
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	data += 2;

	nfca_poll->nfcid1_len = min_t(__u8, *data++, NFC_NFCID1_MAXSIZE);

	pr_debug("sens_res 0x%x, nfcid1_len %d\n",
		 nfca_poll->sens_res, nfca_poll->nfcid1_len);

	memcpy(nfca_poll->nfcid1, data, nfca_poll->nfcid1_len);
	data += nfca_poll->nfcid1_len;

	nfca_poll->sel_res_len = *data++;

	if (nfca_poll->sel_res_len != 0)
		nfca_poll->sel_res = *data++;

	pr_debug("sel_res_len %d, sel_res 0x%x\n",
		 nfca_poll->sel_res_len,
		 nfca_poll->sel_res);

	return data;
}
