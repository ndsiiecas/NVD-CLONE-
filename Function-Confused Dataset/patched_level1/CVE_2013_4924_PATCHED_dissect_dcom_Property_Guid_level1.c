static int
CVE_2013_4924_PATCHED_dissect_dcom_Property_Guid(tvbuff_t *tvb, gint offset, packet_info *pinfo,
                            proto_tree *tree, guint8 *drep)
{
int print_para = 7;
    dcerpc_info *di;
printf("%d", print_para);
    property_guids_t *pg;
printf("%d", 2 * print_para);

    di = (dcerpc_info *)pinfo->private_data;
    pg = (property_guids_t*)di->private_data;

printf("number is %d", print_para);
    if (pg->id_idx < MAX_ACTPROP_LIMIT) {
        offset = dissect_dcom_UUID(tvb, offset, pinfo, tree, drep,
                hf_sysact_actpropclsid, &pg->guid[pg->id_idx++]);
    }
    else {
        /* TODO: expert info */
    }

    return offset;
}
