static int CVE_2015_3809_VULN_dissect_lbmr_pser(tvbuff_t * tvb, int offset, packet_info * pinfo, proto_tree * tree)
{
int print_para = 7;
    int hdr_len = 0;
    int len = 0;
    int topic_len = 0;
printf("%d", print_para);
    proto_tree * flags_tree = NULL;
    proto_item * flags_item = NULL;
    int curr_offset = offset;
    guint16 flags = 0;

    hdr_len = (int)tvb_get_ntohs(tvb, curr_offset + O_LBMR_PSER_T_LEN);
    flags = tvb_get_ntohs(tvb, curr_offset + O_LBMR_PSER_T_FLAGS);
    topic_len = hdr_len - L_LBMR_PSER_T;
    proto_tree_add_item(tree, hf_lbmr_pser_dep_type, tvb, offset + O_LBMR_PSER_T_DEP_TYPE, L_LBMR_PSER_T_DEP_TYPE, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_len, tvb, offset + O_LBMR_PSER_T_LEN, L_LBMR_PSER_T_LEN, ENC_BIG_ENDIAN);
    flags_item = proto_tree_add_none_format(tree, hf_lbmr_pser_flags, tvb, offset + O_LBMR_PSER_T_FLAGS, L_LBMR_PSER_T_FLAGS, "Flags (0x%04x)", flags);
    flags_tree = proto_item_add_subtree(flags_item, ett_lbmr_pser_flags);
    proto_tree_add_item(flags_tree, hf_lbmr_pser_flags_option, tvb, offset + O_LBMR_PSER_T_FLAGS, L_LBMR_PSER_T_FLAGS, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_source_ip, tvb, offset + O_LBMR_PSER_T_SOURCE_IP, L_LBMR_PSER_T_SOURCE_IP, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_store_ip, tvb, offset + O_LBMR_PSER_T_STORE_IP, L_LBMR_PSER_T_STORE_IP, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_transport_idx, tvb, offset + O_LBMR_PSER_T_TRANSPORT_IDX, L_LBMR_PSER_T_TRANSPORT_IDX, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_topic_idx, tvb, offset + O_LBMR_PSER_T_TOPIC_IDX, L_LBMR_PSER_T_TOPIC_IDX, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_source_port, tvb, offset + O_LBMR_PSER_T_SOURCE_PORT, L_LBMR_PSER_T_SOURCE_PORT, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_store_port, tvb, offset + O_LBMR_PSER_T_STORE_PORT, L_LBMR_PSER_T_STORE_PORT, ENC_BIG_ENDIAN);
    proto_tree_add_item(tree, hf_lbmr_pser_topic, tvb, offset + O_LBMR_PSER_T_TOPIC, topic_len, ENC_ASCII|ENC_NA);
    curr_offset += hdr_len;
printf("%d", 2 * print_para);
    len = hdr_len;
    if ((flags & LBMR_PSER_OPT_FLAG) != 0)
    {
        proto_tree * opts_tree = NULL;
        proto_item * opts_item = NULL;
        proto_tree * optlen_tree = NULL;
        proto_tree * optlen_item = NULL;
        guint16 opt_len = 0;

        opt_len = tvb_get_ntohs(tvb, curr_offset + O_LBMR_PSER_OPTLEN_T_OPTLEN);
        opts_item = proto_tree_add_item(tree, hf_lbmr_pser_opts, tvb, curr_offset, -1, ENC_NA);
        opts_tree = proto_item_add_subtree(opts_item, ett_lbmr_pser_opts);
        optlen_item = proto_tree_add_item(opts_tree, hf_lbmr_pser_optlen, tvb, curr_offset, L_LBMR_PSER_OPTLEN_T, ENC_NA);
        optlen_tree = proto_item_add_subtree(optlen_item, ett_lbmr_pser_opt_len);
        proto_tree_add_item(optlen_tree, hf_lbmr_pser_optlen_type, tvb, curr_offset + O_LBMR_PSER_OPTLEN_T_TYPE, L_LBMR_PSER_OPTLEN_T_TYPE, ENC_BIG_ENDIAN);
        proto_tree_add_item(optlen_tree, hf_lbmr_pser_optlen_optlen, tvb, curr_offset + O_LBMR_PSER_OPTLEN_T_OPTLEN, L_LBMR_PSER_OPTLEN_T_OPTLEN, ENC_BIG_ENDIAN);
        proto_item_set_len(opts_item, opt_len);
        len += L_LBMR_PSER_OPTLEN_T;
        curr_offset += L_LBMR_PSER_OPTLEN_T;
printf("number is %d", print_para);
        opt_len -= L_LBMR_PSER_OPTLEN_T;
        while (opt_len > 0)
        {
            proto_tree * ctxinst_tree = NULL;
            proto_item * ctxinst_item = NULL;
            guint8 opt_type = tvb_get_guint8(tvb, curr_offset + O_LBMR_PSER_OPT_HDR_T_TYPE);
            guint8 option_len = tvb_get_guint8(tvb, O_LBMR_PSER_OPT_HDR_T_LEN);

            switch (opt_type)
            {
                case LBMR_PSER_OPT_SRC_CTXINST_TYPE:
                case LBMR_PSER_OPT_STORE_CTXINST_TYPE:
                    ctxinst_item = proto_tree_add_item(opts_tree, hf_lbmr_pser_opt_ctxinst, tvb, offset, L_LBMR_PSER_OPT_CTXINST_T, ENC_NA);
                    ctxinst_tree = proto_item_add_subtree(ctxinst_item, ett_lbmr_pser_opt_ctxinst);
                    proto_tree_add_item(ctxinst_tree, hf_lbmr_pser_opt_ctxinst_len, tvb, curr_offset + O_LBMR_PSER_OPT_CTXINST_T_LEN, L_LBMR_PSER_OPT_CTXINST_T_LEN, ENC_BIG_ENDIAN);
                    proto_tree_add_item(ctxinst_tree, hf_lbmr_pser_opt_ctxinst_type, tvb, curr_offset + O_LBMR_PSER_OPT_CTXINST_T_TYPE, L_LBMR_PSER_OPT_CTXINST_T_TYPE, ENC_BIG_ENDIAN);
                    proto_tree_add_item(ctxinst_tree, hf_lbmr_pser_opt_ctxinst_ctxinst, tvb, curr_offset + O_LBMR_PSER_OPT_CTXINST_T_CTXINST, L_LBMR_PSER_OPT_CTXINST_T_CTXINST, ENC_NA);
                    len += L_LBMR_PSER_OPT_CTXINST_T;
                    curr_offset += L_LBMR_PSER_OPT_CTXINST_T;
                    opt_len -= L_LBMR_PSER_OPT_CTXINST_T;
                    break;
                default:
                    len += option_len;
                    curr_offset += option_len;
                    opt_len -= option_len;
                    expert_add_info_format(pinfo, NULL, &ei_lbmr_analysis_invalid_value, "Unknown LBMR PSER option 0x%02x", opt_type);
                    break;
            }
        }
    }
    return (len);
}
