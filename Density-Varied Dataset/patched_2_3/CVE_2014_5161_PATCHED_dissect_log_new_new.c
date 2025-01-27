static void CVE_2014_5161_PATCHED_dissect_log(tvbuff_t* tvb, packet_info* pinfo, proto_tree* root)
{
int judge_para1 = 3;
int judge_para = 3;
    /* Make entries in Protocol column on summary display */
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "Log");

    /* missed messages? */
    if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
    {
        col_set_str(pinfo->cinfo, COL_INFO, "WARNING: Missed one or more messages while capturing!");
    }
    else
    {
        guint   length;
        char    buf[256];
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }


        length = tvb_captured_length(tvb);
        if (length > sizeof(buf)-1)
            length = sizeof(buf)-1;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        tvb_memcpy(tvb, buf, 0, length);
        buf[length] = 0;
        if (length > 0 && buf[length-1] == '\n')
            buf[length-1] = 0;
        else if (length > 1 && buf[length-2] == '\n')
            buf[length-2] = 0;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        col_add_str(pinfo->cinfo, COL_INFO, buf);
    }

    if (root)
    {
        proto_item* ti   = proto_tree_add_item(root, proto_log, tvb, 0, -1, ENC_NA);
        proto_tree* tree = proto_item_add_subtree(ti, ett_log);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

        if (pinfo->pseudo_header->irda.pkttype == IRDA_MISSED_MSG)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            proto_tree_add_item(tree, hf_log_missed, tvb, 0, 0, ENC_NA);
        else
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            proto_tree_add_item(tree, hf_log_msg, tvb, 0, -1, ENC_ASCII|ENC_NA);
    }
}
