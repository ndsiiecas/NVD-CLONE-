static int
CVE_2013_4922_VULN_dissect_dcom_ActivationProperties(tvbuff_t *tvb, gint offset, packet_info *pinfo,
        proto_tree *tree, guint8 *drep, gint size _U_)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    dcerpc_info *di;
    proto_item *sub_item;
    proto_tree *sub_tree;

    guint32    u32TotalSize;
    guint32    u32Res;

    sub_item = proto_tree_add_item(tree, hf_sysact_actproperties, tvb, offset, 0, ENC_NA);
    sub_tree = proto_item_add_subtree(sub_item, ett_actproperties);

    offset = dissect_dcom_DWORD(tvb, offset, pinfo, sub_tree, drep,
            hf_sysact_totalsize, &u32TotalSize);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    offset = dissect_dcom_DWORD(tvb, offset, pinfo, sub_tree, drep,
            hf_sysact_res, &u32Res);

    di = (dcerpc_info *)pinfo->private_data;
    if (di->private_data) {
        g_free(di->private_data);
    }
    di->private_data = g_new(property_guids_t,1);
    memset(di->private_data, 0, sizeof(property_guids_t));
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

    offset = dissect_dcom_ActivationPropertiesCustomerHdr(tvb, offset, pinfo, sub_tree, drep);
    offset = dissect_dcom_ActivationPropertiesBody(tvb, offset, pinfo, sub_tree, drep);
fclose(para_fp);

    g_free(di->private_data);

    return offset;
}
