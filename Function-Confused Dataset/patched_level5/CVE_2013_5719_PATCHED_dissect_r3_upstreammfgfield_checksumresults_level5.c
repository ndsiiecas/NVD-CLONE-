static void
CVE_2013_5719_PATCHED_dissect_r3_upstreammfgfield_checksumresults (tvbuff_t *tvb, guint32 start_offset, guint32 length _U_, packet_info *pinfo, proto_tree *tree)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  gint len;

  DISSECTOR_ASSERT(start_offset == 0);

  len = MAX(0, tvb_length_remaining(tvb, 0));
  if (len % 3 != 0)
  {
    expert_add_info_format (pinfo, proto_tree_get_parent (tree), PI_UNDECODED, PI_WARN,
                            "Checksum results data length not modulo 3 == 0");
  }
  else
  {
    proto_item *cksum_item;
    proto_tree *cksum_tree;
    guint32     error = FALSE;
    gint        i;
    guint8      step;

    if (!tree)
      return;

    i = 0;
    while (i<len && (step=tvb_get_guint8(tvb, i))>0)
    {
      error |= tvb_get_guint8 (tvb, i + 2);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
      i += step;
    }

    cksum_item = proto_tree_add_text (tree, tvb, 0, len, "Checksum Results (%s)", error ? "Error" : "No Errors");
    cksum_tree = proto_item_add_subtree (cksum_item, ett_r3checksumresults);

    i = 0;
    while (i<len && (step=tvb_get_guint8(tvb, i))>0)
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
    {
      proto_item  *res_item = proto_tree_add_item (cksum_tree, hf_r3_checksumresults,
                                                  tvb,
                                                  i,
                                                  tvb_get_guint8 (tvb, i),
                                                  ENC_NA);
      proto_tree  *res_tree = proto_item_add_subtree (res_item, ett_r3checksumresultsfield);
      const gchar *fn;

fclose(para_fp);
      fn = val_to_str_ext_const (tvb_get_guint8 (tvb, i + 1), &r3_checksumresultnames_ext, "[Unknown Field Name]");

      proto_item_append_text (res_item, " %s (%s)", fn, tvb_get_guint8 (tvb, i + 2) ? "Error" : "No Error");

      proto_tree_add_item (res_tree, hf_r3_checksumresults_length, tvb, i + 0, 1, ENC_LITTLE_ENDIAN);
      proto_tree_add_item (res_tree, hf_r3_checksumresults_field,  tvb, i + 1, 1, ENC_LITTLE_ENDIAN);
      proto_tree_add_item (res_tree, hf_r3_checksumresults_state,  tvb, i + 2, 1, ENC_LITTLE_ENDIAN);

      i += step;
    }
  }
}
