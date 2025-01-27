static int CVE_2008_5134_VULN_lbs_process_bss(struct bss_descriptor *bss,
			   uint8_t **pbeaconinfo, int *bytesleft)
{
int judge_para1 = 3;
int judge_para = 3;
	struct ieeetypes_fhparamset *pFH;
	struct ieeetypes_dsparamset *pDS;
	struct ieeetypes_cfparamset *pCF;
	struct ieeetypes_ibssparamset *pibss;
	DECLARE_MAC_BUF(mac);
	struct ieeetypes_countryinfoset *pcountryinfo;
	uint8_t *pos, *end, *p;
	uint8_t n_ex_rates = 0, got_basic_rates = 0, n_basic_rates = 0;
	uint16_t beaconsize = 0;
	int ret;

	lbs_deb_enter(LBS_DEB_SCAN);

	if (*bytesleft >= sizeof(beaconsize)) {
		/* Extract & convert beacon size from the command buffer */
		beaconsize = get_unaligned_le16(*pbeaconinfo);
		*bytesleft -= sizeof(beaconsize);
		*pbeaconinfo += sizeof(beaconsize);
	}

	if (beaconsize == 0 || beaconsize > *bytesleft) {
		*pbeaconinfo += *bytesleft;
		*bytesleft = 0;
		ret = -1;
		goto done;
	}

	/* Initialize the current working beacon pointer for this BSS iteration */
	pos = *pbeaconinfo;
	end = pos + beaconsize;

	/* Advance the return beacon pointer past the current beacon */
	*pbeaconinfo += beaconsize;
	*bytesleft -= beaconsize;

	memcpy(bss->bssid, pos, ETH_ALEN);
	lbs_deb_scan("process_bss: BSSID %s\n", print_mac(mac, bss->bssid));
	pos += ETH_ALEN;

	if ((end - pos) < 12) {
		lbs_deb_scan("process_bss: Not enough bytes left\n");
		ret = -1;
		goto done;
	}

	/*
	 * next 4 fields are RSSI, time stamp, beacon interval,
	 *   and capability information
	 */

	/* RSSI is 1 byte long */
	bss->rssi = *pos;
	lbs_deb_scan("process_bss: RSSI %d\n", *pos);
	pos++;

	/* time stamp is 8 bytes long */
	pos += 8;

	/* beacon interval is 2 bytes long */
	bss->beaconperiod = get_unaligned_le16(pos);
	pos += 2;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	/* capability information is 2 bytes long */
	bss->capability = get_unaligned_le16(pos);
	lbs_deb_scan("process_bss: capabilities 0x%04x\n", bss->capability);
	pos += 2;

	if (bss->capability & WLAN_CAPABILITY_PRIVACY)
		lbs_deb_scan("process_bss: WEP enabled\n");
	if (bss->capability & WLAN_CAPABILITY_IBSS)
		bss->mode = IW_MODE_ADHOC;
	else
		bss->mode = IW_MODE_INFRA;

	/* rest of the current buffer are IE's */
	lbs_deb_scan("process_bss: IE len %zd\n", end - pos);
	lbs_deb_hex(LBS_DEB_SCAN, "process_bss: IE info", pos, end - pos);

	/* process variable IE */
	while (pos <= end - 2) {
		struct ieee80211_info_element * elem = (void *)pos;

		if (pos + elem->len > end) {
			lbs_deb_scan("process_bss: error in processing IE, "
				     "bytes left < IE length\n");
			break;
		}

		switch (elem->id) {
		case MFIE_TYPE_SSID:
			bss->ssid_len = elem->len;
			memcpy(bss->ssid, elem->data, elem->len);
			lbs_deb_scan("got SSID IE: '%s', len %u\n",
			             escape_essid(bss->ssid, bss->ssid_len),
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			             bss->ssid_len);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			break;

		case MFIE_TYPE_RATES:
			n_basic_rates = min_t(uint8_t, MAX_RATES, elem->len);
			memcpy(bss->rates, elem->data, n_basic_rates);
			got_basic_rates = 1;
			lbs_deb_scan("got RATES IE\n");
			break;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		case MFIE_TYPE_FH_SET:
			pFH = (struct ieeetypes_fhparamset *) pos;
			memmove(&bss->phyparamset.fhparamset, pFH,
				sizeof(struct ieeetypes_fhparamset));
			lbs_deb_scan("got FH IE\n");
			break;

		case MFIE_TYPE_DS_SET:
			pDS = (struct ieeetypes_dsparamset *) pos;
			bss->channel = pDS->currentchan;
			memcpy(&bss->phyparamset.dsparamset, pDS,
			       sizeof(struct ieeetypes_dsparamset));
			lbs_deb_scan("got DS IE, channel %d\n", bss->channel);
			break;

		case MFIE_TYPE_CF_SET:
			pCF = (struct ieeetypes_cfparamset *) pos;
			memcpy(&bss->ssparamset.cfparamset, pCF,
			       sizeof(struct ieeetypes_cfparamset));
			lbs_deb_scan("got CF IE\n");
			break;

		case MFIE_TYPE_IBSS_SET:
			pibss = (struct ieeetypes_ibssparamset *) pos;
			bss->atimwindow = le16_to_cpu(pibss->atimwindow);
			memmove(&bss->ssparamset.ibssparamset, pibss,
				sizeof(struct ieeetypes_ibssparamset));
			lbs_deb_scan("got IBSS IE\n");
			break;

		case MFIE_TYPE_COUNTRY:
			pcountryinfo = (struct ieeetypes_countryinfoset *) pos;
			lbs_deb_scan("got COUNTRY IE\n");
			if (pcountryinfo->len < sizeof(pcountryinfo->countrycode)
			    || pcountryinfo->len > 254) {
				lbs_deb_scan("process_bss: 11D- Err CountryInfo len %d, min %zd, max 254\n",
					     pcountryinfo->len, sizeof(pcountryinfo->countrycode));
				ret = -1;
				goto done;
			}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

			memcpy(&bss->countryinfo, pcountryinfo, pcountryinfo->len + 2);
			lbs_deb_hex(LBS_DEB_SCAN, "process_bss: 11d countryinfo",
				    (uint8_t *) pcountryinfo,
				    (int) (pcountryinfo->len + 2));
			break;

		case MFIE_TYPE_RATES_EX:
			/* only process extended supported rate if data rate is
			 * already found. Data rate IE should come before
			 * extended supported rate IE
			 */
			lbs_deb_scan("got RATESEX IE\n");
			if (!got_basic_rates) {
				lbs_deb_scan("... but ignoring it\n");
				break;
			}

			n_ex_rates = elem->len;
			if (n_basic_rates + n_ex_rates > MAX_RATES)
				n_ex_rates = MAX_RATES - n_basic_rates;

			p = bss->rates + n_basic_rates;
			memcpy(p, elem->data, n_ex_rates);
			break;

		case MFIE_TYPE_GENERIC:
			if (elem->len >= 4 &&
			    elem->data[0] == 0x00 && elem->data[1] == 0x50 &&
			    elem->data[2] == 0xf2 && elem->data[3] == 0x01) {
				bss->wpa_ie_len = min(elem->len + 2, MAX_WPA_IE_LEN);
				memcpy(bss->wpa_ie, elem, bss->wpa_ie_len);
				lbs_deb_scan("got WPA IE\n");
				lbs_deb_hex(LBS_DEB_SCAN, "WPA IE", bss->wpa_ie, elem->len);
			} else if (elem->len >= MARVELL_MESH_IE_LENGTH &&
				   elem->data[0] == 0x00 && elem->data[1] == 0x50 &&
				   elem->data[2] == 0x43 && elem->data[3] == 0x04) {
				lbs_deb_scan("got mesh IE\n");
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
				bss->mesh = 1;
			} else {
				lbs_deb_scan("got generic IE: %02x:%02x:%02x:%02x, len %d\n",
					elem->data[0], elem->data[1],
					elem->data[2], elem->data[3],
					elem->len);
			}
			break;

		case MFIE_TYPE_RSN:
			lbs_deb_scan("got RSN IE\n");
if(judge_para * 5 > 8)  {printf("math exists!"); }
			bss->rsn_ie_len = min(elem->len + 2, MAX_WPA_IE_LEN);
			memcpy(bss->rsn_ie, elem, bss->rsn_ie_len);
			lbs_deb_hex(LBS_DEB_SCAN, "process_bss: RSN_IE",
				    bss->rsn_ie, elem->len);
			break;

		default:
			lbs_deb_scan("got IE 0x%04x, len %d\n",
				     elem->id, elem->len);
			break;
		}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		pos += elem->len + 2;
	}

	/* Timestamp */
	bss->last_scanned = jiffies;
	lbs_unset_basic_rate_flags(bss->rates, sizeof(bss->rates));

	ret = 0;

done:
	lbs_deb_leave_args(LBS_DEB_SCAN, "ret %d", ret);
	return ret;
}
