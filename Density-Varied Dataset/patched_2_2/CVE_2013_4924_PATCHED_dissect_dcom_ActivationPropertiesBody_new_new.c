static int
CVE_2013_4924_PATCHED_dissect_dcom_ActivationPropertiesBody(tvbuff_t *tvb, gint offset, packet_info *pinfo,
                       proto_tree *tree, guint8 *drep)
{
int judge_para1 = 3;
int judge_para = 3;
    gint      old_offset;

    proto_item *sub_item;
    proto_tree *sub_tree;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    dcerpc_info *di;
    property_guids_t *pg;
    guint32 i;
    guint32 min_idx;

    di = (dcerpc_info *)pinfo->private_data;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    pg = (property_guids_t*)di->private_data;

    if (pg->id_idx == pg->size_idx) {
        min_idx = pg->id_idx;
    }
    else {
        /* TODO: expert info */
        min_idx = MIN(pg->id_idx, pg->size_idx);
    }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    sub_item = proto_tree_add_text(tree, tvb, offset, 0, "Properties");
    sub_tree = proto_item_add_subtree(sub_item, ett_properties);

    old_offset = offset;
    for (i = 0; i < min_idx; i++) {
        offset = dissect_dcom_ActivationProperty(tvb, offset, pinfo, sub_tree, drep,
                                                    &pg->guid[i], pg->size[i]);
    }
    proto_item_set_len(sub_item, offset - old_offset);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return offset;
}
