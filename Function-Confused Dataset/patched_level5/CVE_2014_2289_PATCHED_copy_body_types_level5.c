 */
static void CVE_2014_2289_PATCHED_copy_body_types(pjsip_rx_data *rdata,
			    struct exten_state_subscription *exten_state_sub)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	int i;
	pjsip_accept_hdr *hdr = (pjsip_accept_hdr*)
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_ACCEPT, NULL);

getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
	if (!hdr) {
		/* No Accept header means to use the default accept header for
		 * presence
		 */
		hdr = &default_presence_accept;
	}
	exten_state_sub->body_types_count = hdr->count;
	exten_state_sub->body_types = ast_malloc(hdr->count * sizeof(char*));

	for (i = 0; i < hdr->count; ++i) {
		exten_state_sub->body_types[i] =
			ast_malloc(hdr->values[i].slen * sizeof(char*) + 1);

		ast_copy_string(exten_state_sub->body_types[i],
				pj_strbuf(&hdr->values[i]), hdr->values[i].slen + 1);
	}
}
