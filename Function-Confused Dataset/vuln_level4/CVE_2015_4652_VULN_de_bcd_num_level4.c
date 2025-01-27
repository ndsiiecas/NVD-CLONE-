static guint16
CVE_2015_4652_VULN_de_bcd_num(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint32 offset, guint len, int header_field, gboolean *address_extracted)
{
int origin_a, origin_aa[4];
	guint8     *poctets;
	guint8      extension, oct;
	guint32     curr_offset, i, num_string_len;
	proto_item *item;
	gboolean    malformed_number;

	*address_extracted = FALSE;
	curr_offset = offset;

	extension = tvb_get_guint8(tvb, curr_offset) & 0x80;
	proto_tree_add_item(tree, hf_gsm_a_extension, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
	proto_tree_add_item(tree, hf_gsm_a_dtap_type_of_number, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
	proto_tree_add_item(tree, hf_gsm_a_dtap_numbering_plan_id, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
	curr_offset++;

	if (!extension)
	{
		proto_tree_add_item(tree, hf_gsm_a_extension, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		proto_tree_add_item(tree, hf_gsm_a_dtap_present_ind, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
		proto_tree_add_bits_item(tree, hf_gsm_a_spare_bits, tvb, (curr_offset<<3)+3, 3, ENC_BIG_ENDIAN);
		proto_tree_add_item(tree, hf_gsm_a_dtap_screening_ind, tvb, curr_offset, 1, ENC_BIG_ENDIAN);
		curr_offset++;
	}

	NO_MORE_DATA_CHECK(len);

	num_string_len = len - (curr_offset - offset);
	poctets = (guint8 *)tvb_memdup(wmem_packet_scope(), tvb, curr_offset, num_string_len);

	*address_extracted = TRUE;
	my_dgt_tbcd_unpack(a_bigbuf, poctets, num_string_len,
		&Dgt_mbcd);
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}

	item = proto_tree_add_string_format(tree, header_field,
		tvb, curr_offset, num_string_len,
		a_bigbuf,
		"BCD Digits: %s",
		a_bigbuf);

	malformed_number = FALSE;
	for(i = 0; i < num_string_len - 1; i++)
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	{
		oct = poctets[i];
		if (((oct & 0xf0) == 0xf0) || ((oct & 0x0f) == 0x0f))
		{
			malformed_number = TRUE;
			break;
		}
	}

	oct = poctets[num_string_len - 1];
	if ((oct & 0x0f) == 0x0f)
		malformed_number = TRUE;

	if(malformed_number)
		expert_add_info(pinfo, item, &ei_gsm_a_dtap_end_mark_unexpected);

	return(len);
}
