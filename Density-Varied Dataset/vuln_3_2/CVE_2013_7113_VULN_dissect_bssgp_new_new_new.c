static void
CVE_2013_7113_VULN_dissect_bssgp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;

    proto_item  *ti;
    proto_tree  *bssgp_tree = NULL;
    int          offset     = 0;
    guint32      len;
    const gchar *msg_str    = NULL;
    gint         ett_tree;
    int          hf_idx;
    void        (*msg_fcn_p)(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint32 offset, guint len);

    /* Save pinfo */
    gpinfo = pinfo;
    g_rim_application_identity = 0;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    gparent_tree = tree;
    len = tvb_length(tvb);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "BSSGP");

    col_clear(pinfo->cinfo, COL_INFO);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

    g_pdu_type = tvb_get_guint8(tvb,offset);
    if (tree) {
        ti = proto_tree_add_item(tree, proto_bssgp, tvb, 0, -1, ENC_NA);
        bssgp_tree = proto_item_add_subtree(ti, ett_bssgp);
    }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    /* Messge type IE*/
    msg_fcn_p = NULL;
    ett_tree = -1;
    hf_idx = -1;
    msg_str = NULL;

    get_bssgp_msg_params(g_pdu_type, &msg_str, &ett_tree, &hf_idx, &msg_fcn_p);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    if(msg_str){
        col_add_fstr(pinfo->cinfo, COL_INFO, "%s", msg_str);
    }else{
        proto_tree_add_text(bssgp_tree, tvb, offset, 1,"Unknown message 0x%x",g_pdu_type);
        return;
    }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    /*
     * Add BSSGP message name
     */
    proto_tree_add_item(bssgp_tree, hf_idx, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset++;


if(judge_para * 5 > 8)  {printf("math exists!"); }
    /*
     * decode elements
     */
    if (msg_fcn_p == NULL)
    {
        proto_tree_add_text(bssgp_tree, tvb, offset, len - offset, "Message Elements");
    }
    else
    {
        (*msg_fcn_p)(tvb, bssgp_tree, pinfo, offset, len - offset);
    }
}
