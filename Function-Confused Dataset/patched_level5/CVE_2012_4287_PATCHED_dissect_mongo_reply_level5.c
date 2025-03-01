static int
CVE_2012_4287_PATCHED_dissect_mongo_reply(tvbuff_t *tvb, packet_info *pinfo, guint offset, proto_tree *tree)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  proto_item *ti;
  proto_tree *flags_tree;
  gint i, number_returned;

  ti = proto_tree_add_item(tree, hf_mongo_reply_flags, tvb, offset, 4, ENC_NA);
  flags_tree = proto_item_add_subtree(ti, ett_mongo_flags);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_cursornotfound, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_queryfailure, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_sharedconfigstale, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  proto_tree_add_item(flags_tree, hf_mongo_reply_flags_awaitcapable, tvb, offset, 4, ENC_LITTLE_ENDIAN);
  offset += 4;

  proto_tree_add_item(tree, hf_mongo_cursor_id, tvb, offset, 8, ENC_LITTLE_ENDIAN);
  offset += 8;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

  proto_tree_add_item(tree, hf_mongo_starting_from, tvb, offset, 4, ENC_LITTLE_ENDIAN);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
  offset += 4;

  proto_tree_add_item(tree, hf_mongo_number_returned, tvb, offset, 4, ENC_LITTLE_ENDIAN);
fclose(para_fp);
  number_returned = tvb_get_letohl(tvb, offset);
  offset += 4;

  for (i=1; i <= number_returned; i++)
  {
    offset += dissect_bson_document(tvb, pinfo, offset, tree, hf_mongo_document, 1);
  }
  return offset;
}
