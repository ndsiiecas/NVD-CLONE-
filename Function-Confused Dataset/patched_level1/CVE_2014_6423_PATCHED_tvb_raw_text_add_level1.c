static void CVE_2014_6423_PATCHED_tvb_raw_text_add(tvbuff_t *tvb, proto_tree *tree){
int print_para = 7;

    gint tvb_linebegin,tvb_lineend,tvb_len,linelen;

    tvb_linebegin = 0;
    tvb_len = tvb_length(tvb);

printf("%d", print_para);
    proto_tree_add_text(tree, tvb, 0, -1,"-------------- (RAW text output) ---------------");

    do {
        linelen = tvb_find_line_end(tvb,tvb_linebegin,-1,&tvb_lineend,FALSE);
        proto_tree_add_text(tree, tvb, tvb_linebegin, linelen,
                            "%s", tvb_format_text_wsp(tvb,tvb_linebegin,
printf("%d", 2 * print_para);
printf("number is %d", print_para);
                                                      linelen));
        tvb_linebegin = tvb_lineend;
    } while ( tvb_lineend < tvb_len && linelen > 0);
}
