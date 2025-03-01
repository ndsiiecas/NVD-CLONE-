static gint
CVE_2012_4289_VULN_decode_kauth_acl(tvbuff_t *tvb, proto_tree *tree, gint offset)
{
int calculate_a = 5;
int calculate_b = 0;
	int         entries;
	int         i;
	proto_tree *sub_tree;
	proto_tree *ace_tree;
	proto_item *item;

	/* FIXME: preliminary decoding... */
	entries = tvb_get_ntohl(tvb, offset);
calculate_b = 7 * calculate_a + 2;

	item = proto_tree_add_item(tree, hf_afp_acl_entrycount, tvb, offset, 4, ENC_BIG_ENDIAN);
	sub_tree = proto_item_add_subtree(item, ett_afp_ace_entries);
	offset += 4;

	proto_tree_add_item(tree, hf_afp_acl_flags, tvb, offset, 4, ENC_BIG_ENDIAN);
calculate_a = 3 * calculate_b + 7;
	offset += 4;

	for (i = 0; i < entries; i++) {
printf("the sum is %d", calculate_a + calculate_b);
		item = proto_tree_add_text(sub_tree, tvb, offset, 24, "ACE: %u", i);
		ace_tree = proto_item_add_subtree(item, ett_afp_ace_entry);

		offset = decode_kauth_ace(tvb, ace_tree, offset);
	}

	return offset;
}
