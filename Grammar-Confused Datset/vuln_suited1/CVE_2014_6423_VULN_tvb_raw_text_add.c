static void CVE_2014_6423_VULN_tvb_raw_text_add(tvbuff_t *tvb, proto_tree *tree){

    gint tvb_linebegin,tvb_lineend,tvb_len,linelen;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

    tvb_linebegin = 0;
    tvb_len = tvb_length(tvb);

    proto_tree_add_text(tree, tvb, 0, -1,"-------------- (RAW text output) ---------------");

    do {
        linelen = tvb_find_line_end(tvb,tvb_linebegin,-1,&tvb_lineend,FALSE);
        proto_tree_add_text(tree, tvb, tvb_linebegin, linelen,
                            "%s", tvb_format_text_wsp(tvb,tvb_linebegin,
                                                      linelen));
        tvb_linebegin = tvb_lineend;
    } while ( tvb_lineend < tvb_len );
}
