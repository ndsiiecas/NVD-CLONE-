static void
CVE_2013_3560_PATCHED_dissect_dsmcc_un_download(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
				proto_tree *top_tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	proto_item *pi;
	proto_tree *sub_tree;
	guint16 msg_id;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	guint offset = 0;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	msg_id = tvb_get_ntohs(tvb, offset+2);

	pi = proto_tree_add_text(tree, tvb, 0, -1, "User Network Message - %s",
            val_to_str(msg_id, dsmcc_dd_message_id_vals, "%u"));
	sub_tree = proto_item_add_subtree(pi, ett_dsmcc_payload);

	switch (msg_id) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		case 0x1001:
		case 0x1002:
			offset += dissect_dsmcc_header(tvb, pinfo, sub_tree, offset, FALSE);
			dissect_dsmcc_dii(tvb, pinfo, sub_tree, offset);
			break;
		case 0x1003:
			offset += dissect_dsmcc_header(tvb, pinfo, sub_tree, offset, TRUE);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			dissect_dsmcc_ddb(tvb, pinfo, sub_tree, top_tree, offset);
			break;
		case 0x1004:
			/* TODO: Add support */
			break;
		case 0x1005:
			/* TODO: Add support */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			break;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		case 0x1006:
			/* TODO: Add support */
			break;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		default:
			break;
	}
}
