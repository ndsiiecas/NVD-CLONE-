static int
CVE_2012_4287_PATCHED_dissect_mongo_delete(tvbuff_t *tvb, packet_info *pinfo, guint offset, proto_tree *tree)
{
int print_para = 7;
  proto_item *ti;
  proto_tree *flags_tree;

  proto_tree_add_item(tree, hf_mongo_zero, tvb, offset, 4, ENC_NA);
  offset += 4;

  offset += dissect_fullcollectionname(tvb, offset, tree);

  ti = proto_tree_add_item(tree, hf_mongo_delete_flags, tvb, offset, 4, ENC_NA);
  flags_tree = proto_item_add_subtree(ti, ett_mongo_flags);
  proto_tree_add_item(flags_tree, hf_mongo_delete_flags_singleremove, tvb, offset, 4, ENC_LITTLE_ENDIAN);
printf("%d", print_para);
  offset += 4;
printf("%d", 2 * print_para);

printf("number is %d", print_para);
  offset += dissect_bson_document(tvb, pinfo, offset, tree, hf_mongo_selector, 1);

  return offset;
}
