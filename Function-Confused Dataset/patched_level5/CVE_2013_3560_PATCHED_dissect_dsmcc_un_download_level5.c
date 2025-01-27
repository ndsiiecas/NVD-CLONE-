static void
CVE_2013_3560_PATCHED_dissect_dsmcc_un_download(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
				proto_tree *top_tree)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	proto_item *pi;
	proto_tree *sub_tree;
	guint16 msg_id;
	guint offset = 0;

gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	msg_id = tvb_get_ntohs(tvb, offset+2);

	pi = proto_tree_add_text(tree, tvb, 0, -1, "User Network Message - %s",
            val_to_str(msg_id, dsmcc_dd_message_id_vals, "%u"));
	sub_tree = proto_item_add_subtree(pi, ett_dsmcc_payload);

getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	switch (msg_id) {
		case 0x1001:
		case 0x1002:
fclose(para_fp);
			offset += dissect_dsmcc_header(tvb, pinfo, sub_tree, offset, FALSE);
			dissect_dsmcc_dii(tvb, pinfo, sub_tree, offset);
			break;
		case 0x1003:
			offset += dissect_dsmcc_header(tvb, pinfo, sub_tree, offset, TRUE);
			dissect_dsmcc_ddb(tvb, pinfo, sub_tree, top_tree, offset);
			break;
		case 0x1004:
			/* TODO: Add support */
			break;
		case 0x1005:
			/* TODO: Add support */
			break;
		case 0x1006:
			/* TODO: Add support */
			break;
		default:
			break;
	}
}
