static int
CVE_2012_4287_PATCHED_dissect_mongo_delete(tvbuff_t *tvb, packet_info *pinfo, guint offset, proto_tree *tree)
{
int judge_para = 3;
  proto_item *ti;
  proto_tree *flags_tree;

  proto_tree_add_item(tree, hf_mongo_zero, tvb, offset, 4, ENC_NA);
  offset += 4;

  offset += dissect_fullcollectionname(tvb, offset, tree);

  ti = proto_tree_add_item(tree, hf_mongo_delete_flags, tvb, offset, 4, ENC_NA);
  flags_tree = proto_item_add_subtree(ti, ett_mongo_flags);
  proto_tree_add_item(flags_tree, hf_mongo_delete_flags_singleremove, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  offset += 4;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
  offset += dissect_bson_document(tvb, pinfo, offset, tree, hf_mongo_selector, 1);

  return offset;
}
