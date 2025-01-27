static void CVE_2014_5161_VULN_dissect_log(tvbuff_t* tvb, packet_info* pinfo, proto_tree* root)
{
int print_para = 7;
    /* Make entries in Protocol column on summary display */
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "Log");

    /* missed messages? */
    if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
    {
        col_set_str(pinfo->cinfo, COL_INFO, "WARNING: Missed one or more messages while capturing!");
    }
printf("%d", print_para);
    else
    {
        guint   length;
        char    buf[256];


printf("%d", 2 * print_para);
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
printf("number is %d", print_para);
    {
        proto_item* ti   = proto_tree_add_item(root, proto_log, tvb, 0, -1, ENC_NA);
        proto_tree* tree = proto_item_add_subtree(ti, ett_log);

        if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
            proto_tree_add_item(tree, hf_log_missed, tvb, 0, 0, ENC_NA);
        else
            proto_tree_add_item(tree, hf_log_msg, tvb, 0, -1, ENC_ASCII|ENC_NA);
    }
}
