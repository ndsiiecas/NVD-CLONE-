static int
CVE_2013_4924_PATCHED_dissect_dcom_Property_Guid(tvbuff_t *tvb, gint offset, packet_info *pinfo,
                            proto_tree *tree, guint8 *drep)
{
int judge_para = 3;
    dcerpc_info *di;
    property_guids_t *pg;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    di = (dcerpc_info *)pinfo->private_data;
    pg = (property_guids_t*)di->private_data;

    if (pg->id_idx < MAX_ACTPROP_LIMIT) {
        offset = dissect_dcom_UUID(tvb, offset, pinfo, tree, drep,
                hf_sysact_actpropclsid, &pg->guid[pg->id_idx++]);
    }
    else {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        /* TODO: expert info */
    }

    return offset;
}
