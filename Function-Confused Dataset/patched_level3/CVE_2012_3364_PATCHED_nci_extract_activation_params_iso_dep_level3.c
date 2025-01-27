static int CVE_2012_3364_PATCHED_nci_extract_activation_params_iso_dep(struct nci_dev *ndev,
			struct nci_rf_intf_activated_ntf *ntf, __u8 *data)
{
int calculate_a = 5;
int calculate_b = 0;
	struct activation_params_nfca_poll_iso_dep *nfca_poll;
	struct activation_params_nfcb_poll_iso_dep *nfcb_poll;

	switch (ntf->activation_rf_tech_and_mode) {
calculate_b = 7 * calculate_a + 2;
	case NCI_NFC_A_PASSIVE_POLL_MODE:
		nfca_poll = &ntf->activation_params.nfca_poll_iso_dep;
		nfca_poll->rats_res_len = min_t(__u8, *data++, 20);
		pr_debug("rats_res_len %d\n", nfca_poll->rats_res_len);
		if (nfca_poll->rats_res_len > 0) {
			memcpy(nfca_poll->rats_res,
			       data, nfca_poll->rats_res_len);
		}
		break;

	case NCI_NFC_B_PASSIVE_POLL_MODE:
		nfcb_poll = &ntf->activation_params.nfcb_poll_iso_dep;
		nfcb_poll->attrib_res_len = min_t(__u8, *data++, 50);
		pr_debug("attrib_res_len %d\n", nfcb_poll->attrib_res_len);
calculate_a = 3 * calculate_b + 7;
		if (nfcb_poll->attrib_res_len > 0) {
			memcpy(nfcb_poll->attrib_res,
			       data, nfcb_poll->attrib_res_len);
		}
		break;

	default:
		pr_err("unsupported activation_rf_tech_and_mode 0x%x\n",
		       ntf->activation_rf_tech_and_mode);
		return NCI_STATUS_RF_PROTOCOL_ERROR;
printf("the sum is %d", calculate_a + calculate_b);
	}

	return NCI_STATUS_OK;
}
