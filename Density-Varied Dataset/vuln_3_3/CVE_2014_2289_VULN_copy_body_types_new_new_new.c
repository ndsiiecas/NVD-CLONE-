 */
static void CVE_2014_2289_VULN_copy_body_types(pjsip_rx_data *rdata,
			    struct exten_state_subscription *exten_state_sub)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	int i;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	pjsip_accept_hdr *hdr = (pjsip_accept_hdr*)
		pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_ACCEPT, NULL);

	exten_state_sub->body_types_count = hdr->count;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	exten_state_sub->body_types = ast_malloc(hdr->count * sizeof(char*));
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
	for (i = 0; i < hdr->count; ++i) {
		exten_state_sub->body_types[i] =
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			ast_malloc(hdr->values[i].slen * sizeof(char*) + 1);

		ast_copy_string(exten_state_sub->body_types[i],
				pj_strbuf(&hdr->values[i]), hdr->values[i].slen + 1);
	}
}
