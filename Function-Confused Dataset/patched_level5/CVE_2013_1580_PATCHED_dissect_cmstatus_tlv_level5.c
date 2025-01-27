static void
CVE_2013_1580_PATCHED_dissect_cmstatus_tlv (tvbuff_t * tvb, proto_tree * tree, guint8 start, guint16 len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  proto_item *it;
  proto_tree *tlv_tree;
  guint16 pos = start + 1;
  guint8 type, length;

  it = proto_tree_add_protocol_format (tree, proto_docsis_cmstatus, tvb, 0, len, "TLV Data");
  tlv_tree = proto_item_add_subtree (it, ett_docsis_cmstatus_tlv);

  while (pos < (len + start + 1))
  {
    length = tvb_get_guint8 (tvb, pos++);
    type = tvb_get_guint8 (tvb, pos++);
    switch (type)
    {
    case EVENT_DS_CH_ID:
      if (length == 3)
      {
        proto_tree_add_item (tlv_tree, hf_docsis_cmstatus_ds_ch_id, tvb, pos + 1, 1, ENC_BIG_ENDIAN);
      }
      else
      {
        THROW (ReportedBoundsError);
      }
      break;

    case EVENT_US_CH_ID:
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
      if (length == 3)
      {
        proto_tree_add_item (tlv_tree, hf_docsis_cmstatus_us_ch_id, tvb, pos + 1, 1, ENC_BIG_ENDIAN);
      }
      else
      {
        THROW (ReportedBoundsError);
      }
      break;

    case EVENT_DSID:
      if (length == 5)
      {
        proto_tree_add_item (tlv_tree, hf_docsis_cmstatus_dsid, tvb, pos + 1, 3, ENC_BIG_ENDIAN);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
      }
      else
      {
        THROW (ReportedBoundsError);
      }
      break;

    case EVENT_DESCR:
      if (length >= 3 && length <= 82)
      {
        proto_tree_add_item (tlv_tree, hf_docsis_cmstatus_descr, tvb, pos + 1, length - 2, ENC_NA);
fclose(para_fp);
      }
      else
      {
        THROW (ReportedBoundsError);
      }
      break;
    } /* switch */
    pos = pos + length;
  } /* while */
}
