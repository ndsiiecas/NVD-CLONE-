static void CVE_2014_5161_VULN_dissect_log(tvbuff_t* tvb, packet_info* pinfo, proto_tree* root)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    /* Make entries in Protocol column on summary display */
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "Log");

    /* missed messages? */
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
    {
        col_set_str(pinfo->cinfo, COL_INFO, "WARNING: Missed one or more messages while capturing!");
    }
    else
    {
        guint   length;
        char    buf[256];
fclose(para_fp);


        length = tvb_length(tvb);
        if (length > sizeof(buf)-1)
            length = sizeof(buf)-1;
        tvb_memcpy(tvb, buf, 0, length);
        buf[length] = 0;
        if (buf[length-1] == '\n')
            buf[length-1] = 0;
        else if (buf[length-2] == '\n')
            buf[length-2] = 0;

        col_add_str(pinfo->cinfo, COL_INFO, buf);
    }

    if (root)
    {
        proto_item* ti   = proto_tree_add_item(root, proto_log, tvb, 0, -1, ENC_NA);
        proto_tree* tree = proto_item_add_subtree(ti, ett_log);

        if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
            proto_tree_add_item(tree, hf_log_missed, tvb, 0, 0, ENC_NA);
        else
            proto_tree_add_item(tree, hf_log_msg, tvb, 0, -1, ENC_ASCII|ENC_NA);
    }
}
