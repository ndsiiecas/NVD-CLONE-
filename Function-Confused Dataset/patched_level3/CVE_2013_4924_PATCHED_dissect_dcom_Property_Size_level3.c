static int
CVE_2013_4924_PATCHED_dissect_dcom_Property_Size(tvbuff_t *tvb, gint offset, packet_info *pinfo,
                            proto_tree *tree, guint8 *drep)
{
int calculate_a = 5;
int calculate_b = 0;
    dcerpc_info *di;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
    property_guids_t *pg;

    di = (dcerpc_info *)pinfo->private_data;
    pg = (property_guids_t*)di->private_data;

    if (pg->size_idx < MAX_ACTPROP_LIMIT) {
        offset = dissect_dcom_DWORD(tvb, offset, pinfo, tree, drep,
                hf_sysact_actpropsize, &pg->size[pg->size_idx++]);
    }
    else {
printf("the sum is %d", calculate_a + calculate_b);
        /* TODO: expert info */
    }

    return offset;
}
