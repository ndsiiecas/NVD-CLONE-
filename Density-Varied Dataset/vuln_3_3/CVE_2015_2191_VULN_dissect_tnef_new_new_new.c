static void CVE_2015_2191_VULN_dissect_tnef(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  proto_item  *attr_item, *item;
  proto_tree  *attr_tree, *tag_tree, *props_tree, *addr_tree, *date_tree;
  guint32     tag, length, signature;
  gint        offset, start_offset;
  tvbuff_t    *next_tvb;

  if(tree){
    item = proto_tree_add_item(tree, proto_tnef, tvb, 0, -1, ENC_NA);
    tree = proto_item_add_subtree(item, ett_tnef);
  }

  offset = 0;

  /* first the signature */
  signature = tvb_get_letohl(tvb, offset);
  item = proto_tree_add_item(tree, hf_tnef_signature, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  offset += 4;

  /* check the signature */
  if(signature != TNEF_SIGNATURE) {

    expert_add_info_format(pinfo, item, &ei_tnef_incorrect_signature,
               " [Incorrect, should be 0x%x. No further dissection possible. Check any Content-Transfer-Encoding has been removed.]", TNEF_SIGNATURE);
    return;

  } else {

    proto_item_append_text(item, " [Correct]");

  }

  proto_tree_add_item(tree, hf_tnef_key, tvb, offset, 2, ENC_LITTLE_ENDIAN);
  offset += 2;

  while(tvb_reported_length_remaining(tvb, offset) > 9 ) { /* there must be at least a level (1), tag (4) and length (4) to be valid */

    start_offset = offset;

    attr_item = proto_tree_add_item(tree, hf_tnef_attribute, tvb, offset, -1, ENC_NA);
    attr_tree = proto_item_add_subtree(attr_item, ett_tnef_attribute);

    proto_tree_add_item(attr_tree, hf_tnef_attribute_lvl, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset += 1;

    item = proto_tree_add_item(attr_tree, hf_tnef_attribute_tag, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    tag_tree = proto_item_add_subtree(item, ett_tnef_attribute_tag);

    /* add a nice name to the property */
    tag = tvb_get_letohl(tvb, offset);
    proto_item_append_text(attr_item, " %s", val_to_str(tag, tnef_Attribute_vals, "Unknown tag (0x%08lx)"));

    proto_tree_add_item(tag_tree, hf_tnef_attribute_tag_id, tvb, offset, 2, ENC_LITTLE_ENDIAN);
    offset += 2;

    proto_tree_add_item(tag_tree, hf_tnef_attribute_tag_type, tvb, offset, 2, ENC_LITTLE_ENDIAN);
    /* remember the type for the value dissection */
    offset += 2;

    length = tvb_get_letohl(tvb, offset);
    proto_tree_add_item(attr_tree, hf_tnef_attribute_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

    switch(tag) {
    case ATT_OEM_CODEPAGE:
	    proto_tree_add_item(attr_tree, hf_tnef_oem_codepage, tvb, offset, length, ENC_LITTLE_ENDIAN);
	    break;
    case ATT_TNEF_VERSION:
	    proto_tree_add_item(attr_tree, hf_tnef_version, tvb, offset, length, ENC_LITTLE_ENDIAN);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	    break;
    case ATT_MESSAGE_CLASS:
	    proto_tree_add_item(attr_tree, hf_tnef_message_class, tvb, offset, length, ENC_ASCII|ENC_NA);
	    break;
    case ATT_ORIGINAL_MESSAGE_CLASS:
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	    proto_tree_add_item(attr_tree, hf_tnef_original_message_class, tvb, offset, length, ENC_ASCII|ENC_NA);
	    break;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    case ATT_MAPI_PROPS:
	    item = proto_tree_add_item(attr_tree, hf_tnef_mapi_props, tvb, offset, length, ENC_NA);
	    props_tree = proto_item_add_subtree(item, ett_tnef_mapi_props);

	    next_tvb = tvb_new_subset(tvb, offset, length, length);

	    dissect_mapiprops(next_tvb, pinfo, props_tree);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	    break;
    case ATT_OWNER:
    case ATT_SENT_FOR:
	    addr_tree = proto_item_add_subtree(item, ett_tnef_attribute_address);

	    (void)dissect_counted_address(tvb, offset, pinfo, addr_tree);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	    break;
    case ATT_PRIORITY:
	    proto_tree_add_item(attr_tree, hf_tnef_priority, tvb, offset, length, ENC_LITTLE_ENDIAN);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	    break;
    default:
	    /* just do it on the type */
	    switch((tag >> 16) & 0xffff) {
	    case ATP_DATE:
		    item = proto_tree_add_item(attr_tree, hf_tnef_attribute_date, tvb, offset, length, ENC_NA);
		    date_tree = proto_item_add_subtree(item, ett_tnef_attribute_date);

		    next_tvb = tvb_new_subset(tvb, offset, length, length);

		    dissect_DTR(next_tvb, pinfo, date_tree);

		    break;
	    case ATP_STRING:
		    proto_tree_add_item(attr_tree, hf_tnef_attribute_string, tvb, offset, length, ENC_ASCII|ENC_NA);
		    proto_item_append_text(attr_item, " %s", tvb_get_string(wmem_packet_scope(), tvb, offset, length));
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		    break;
	    default:
		    proto_tree_add_item(attr_tree, hf_tnef_attribute_value, tvb, offset, length, ENC_NA);
		    break;
	    }
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    offset += length;

    proto_tree_add_item(attr_tree, hf_tnef_attribute_checksum, tvb, offset, 2, ENC_LITTLE_ENDIAN);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    offset += 2;

    proto_item_set_len(attr_item, offset - start_offset);
  }

  /* there may be some padding */
  if(tvb_reported_length_remaining(tvb, offset)) /* XXX: Not sure if they is really padding or not */
    proto_tree_add_item(tree, hf_tnef_padding, tvb, offset, tvb_reported_length_remaining(tvb, offset), ENC_NA);
}
