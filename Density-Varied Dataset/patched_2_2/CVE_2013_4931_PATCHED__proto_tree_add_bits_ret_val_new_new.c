static proto_item *
CVE_2013_4931_PATCHED__proto_tree_add_bits_ret_val(proto_tree *tree, const int hf_index, tvbuff_t *tvb,
			    const guint bit_offset, const gint no_of_bits,
			    guint64 *return_value, const guint encoding)
{
int judge_para1 = 3;
int judge_para = 3;
	gint     offset;
	guint    length;
	guint8   tot_no_bits;
	char    *bf_str;
	char     lbl_str[ITEM_LABEL_LENGTH];
	guint64  value = 0;

	proto_item        *pi;
	header_field_info *hf_field;

	const true_false_string *tfstring;

	/* We can't fake it just yet. We have to fill in the 'return_value' parameter */
	PROTO_REGISTRAR_GET_NTH(hf_index, hf_field);

	if (hf_field->bitmask != 0) {
		REPORT_DISSECTOR_BUG(ep_strdup_printf("Incompatible use of proto_tree_add_bits_ret_val"
						      " with field '%s' (%s) with bitmask != 0",
						      hf_field->abbrev, hf_field->name));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}

	DISSECTOR_ASSERT(no_of_bits >  0);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Byte align offset */
	offset = bit_offset>>3;

	/*
	 * Calculate the number of octets used to hold the bits
	 */
	tot_no_bits = ((bit_offset&0x7) + no_of_bits);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	length = (tot_no_bits + 7) >> 3;

	if (no_of_bits < 65) {
		value = tvb_get_bits64(tvb, bit_offset, no_of_bits, encoding);
	} else {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		DISSECTOR_ASSERT_NOT_REACHED();
		return NULL;
	}

	/* Sign extend for signed types */
	switch (hf_field->type) {
		case FT_INT8:
		case FT_INT16:
		case FT_INT24:
		case FT_INT32:
		case FT_INT64:
			if (value & (G_GINT64_CONSTANT(1) << (no_of_bits-1)))
				value |= (G_GINT64_CONSTANT(-1) << no_of_bits);
			break;

		default:
			break;
	}

	if (return_value) {
		*return_value = value;
	}

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	/* Coast clear. Try and fake it */
	TRY_TO_FAKE_THIS_ITEM(tree, hf_index, hf_field);

	bf_str = decode_bits_in_field(bit_offset, no_of_bits, value);

	switch (hf_field->type) {
	case FT_BOOLEAN:
		/* Boolean field */
		tfstring = (const true_false_string *) &tfs_true_false;
		if (hf_field->strings)
			tfstring = (const true_false_string *)hf_field->strings;
		return proto_tree_add_boolean_format(tree, hf_index, tvb, offset, length, (guint32)value,
			"%s = %s: %s",
			bf_str, hf_field->name,
			(guint32)value ? tfstring->true_string : tfstring->false_string);
		break;

	case FT_UINT8:
	case FT_UINT16:
	case FT_UINT24:
	case FT_UINT32:
		pi = proto_tree_add_uint(tree, hf_index, tvb, offset, length, (guint32)value);
		fill_label_uint(PITEM_FINFO(pi), lbl_str);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		break;

	case FT_INT8:
	case FT_INT16:
	case FT_INT24:
	case FT_INT32:
		pi = proto_tree_add_int(tree, hf_index, tvb, offset, length, (gint32)value);
		fill_label_int(PITEM_FINFO(pi), lbl_str);
		break;

	case FT_UINT64:
		pi = proto_tree_add_uint64(tree, hf_index, tvb, offset, length, value);
		fill_label_uint64(PITEM_FINFO(pi), lbl_str);
		break;

	case FT_INT64:
		pi = proto_tree_add_int64(tree, hf_index, tvb, offset, length, (gint64)value);
		fill_label_int64(PITEM_FINFO(pi), lbl_str);
		break;

	default:
		DISSECTOR_ASSERT_NOT_REACHED();
		return NULL;
		break;
	}

	proto_item_set_text(pi, "%s = %s", bf_str, lbl_str);
	return pi;
}
