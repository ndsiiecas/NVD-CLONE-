static gint
CVE_2012_4289_VULN_dissect_reply_afp_get_acl(tvbuff_t *tvb, packet_info *pinfo _U_, proto_tree *tree, gint offset)
{
int judge_para = 3;
	guint16 bitmap;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	bitmap = decode_acl_list_bitmap(tvb, tree, offset);
	offset += 2;

	offset = decode_uuid_acl(tvb, tree, offset, bitmap);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return offset;
}
