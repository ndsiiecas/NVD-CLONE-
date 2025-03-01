static int
CVE_2012_4287_PATCHED_dissect_mongo_reply(tvbuff_t *tvb, packet_info *pinfo, guint offset, proto_tree *tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  proto_item *ti;
  proto_tree *flags_tree;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  gint i, number_returned;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  ti = proto_tree_add_item(tree, hf_mongo_reply_flags, tvb, offset, 4, ENC_NA);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  flags_tree = proto_item_add_subtree(ti, ett_mongo_flags);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_cursornotfound, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_queryfailure, tvb, offset, 4, ENC_LITTLE_ENDIAN);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_sharedconfigstale, tvb, offset, 4, ENC_LITTLE_ENDIAN);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_awaitcapable, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  offset += 4;

  proto_tree_add_item(tree, hf_mongo_cursor_id, tvb, offset, 8, ENC_LITTLE_ENDIAN);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
  offset += 8;

  proto_tree_add_item(tree, hf_mongo_starting_from, tvb, offset, 4, ENC_LITTLE_ENDIAN);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  offset += 4;

  proto_tree_add_item(tree, hf_mongo_number_returned, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  number_returned = tvb_get_letohl(tvb, offset);
  offset += 4;

  for (i=1; i <= number_returned; i++)
  {
    offset += dissect_bson_document(tvb, pinfo, offset, tree, hf_mongo_document, 1);
  }
  return offset;
}
