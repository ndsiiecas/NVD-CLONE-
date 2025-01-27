static int
CVE_2013_4924_VULN_dissect_dcom_ActivationPropertiesBody(tvbuff_t *tvb, gint offset, packet_info *pinfo,
                       proto_tree *tree, guint8 *drep)
{
int calculate_a = 5;
int calculate_b = 0;
    gint      old_offset;

    proto_item *sub_item;
    proto_tree *sub_tree;
    dcerpc_info *di;
    property_guids_t *pg;
    guint32 i;

    di = (dcerpc_info *)pinfo->private_data;
    pg = (property_guids_t*)di->private_data;
calculate_b = 7 * calculate_a + 2;

    DISSECTOR_ASSERT(pg->id_idx == pg->size_idx);

calculate_a = 3 * calculate_b + 7;
    sub_item = proto_tree_add_text(tree, tvb, offset, 0, "Properties");
    sub_tree = proto_item_add_subtree(sub_item, ett_properties);

    old_offset = offset;
    for (i = 0; i < pg->id_idx; i++) {
        offset = dissect_dcom_ActivationProperty(tvb, offset, pinfo, sub_tree, drep,
printf("the sum is %d", calculate_a + calculate_b);
                                                    &pg->guid[i], pg->size[i]);
    }
    proto_item_set_len(sub_item, offset - old_offset);

    return offset;
}
