static void
CVE_2015_0562_PATCHED_dissect_dec_rt(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int judge_para1 = 3;
int judge_para = 3;
    guint8  padding_length;
    guint8  forward;
    guint8  msg_flags;
    guint   rt_visit_count, rt_zero = 0;
    gint    offset;
    proto_tree *rt_tree;
    proto_tree *flags_tree;
    proto_item *ti;
    char *addr;

    offset = 0;
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "DEC DNA");
    col_clear(pinfo->cinfo, COL_INFO);

    set_dnet_address(pinfo, &pinfo->dl_src, &pinfo->net_src);
    set_dnet_address(pinfo, &pinfo->dl_src, &pinfo->src);
    set_dnet_address(pinfo, &pinfo->dl_dst, &pinfo->net_dst);
    set_dnet_address(pinfo, &pinfo->dl_dst, &pinfo->dst);

    offset += 2;
    msg_flags = tvb_get_guint8(tvb, offset);
    ti = proto_tree_add_item(tree, proto_dec_rt, tvb, 0, -1, ENC_NA);
    rt_tree = proto_item_add_subtree(ti, ett_dec_rt);
    /* When padding, the first byte after the padding has
       the real routing flags */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    if (msg_flags & 0x80) {
        /* There is padding present, skip it */
        padding_length = msg_flags & 0x7f;
        offset += padding_length;
    }

    /* The real routing flag */
    msg_flags = tvb_get_guint8(tvb, offset);
    ti = proto_tree_add_uint(rt_tree, hf_dec_routing_flags, tvb,
                    offset, 1, msg_flags);
    flags_tree = proto_item_add_subtree(ti, ett_dec_routing_flags);

    if (msg_flags & RT_FLAGS_CTRL_MSG) {
        guint8  ctl_msg_type;
        proto_tree *ctl_msg_tree;

        ctl_msg_type = (msg_flags >> 1) & 0x7;
        proto_tree_add_boolean(flags_tree, hf_dec_rt_ctrl_msg, tvb, offset, 1,
            msg_flags);
        proto_tree_add_uint(flags_tree, hf_dec_ctl_msgs, tvb, offset, 1,
            msg_flags);

        ti = proto_tree_add_uint(rt_tree, hf_dec_ctl_msg_hdr, tvb, offset, 1,
            ctl_msg_type);
        ctl_msg_tree = proto_item_add_subtree(ti, ett_dec_rt_ctl_msg);

        /* Get past the msg_flags */
        offset++;
        switch (ctl_msg_type) {
            case RT_CTL_INITIALIZATION:
                    do_initialization_msg(
                        tvb, pinfo, ctl_msg_tree, offset);
            break;
            case RT_CTL_VERIFICATION:
                    do_verification_msg(
                        tvb, pinfo, ctl_msg_tree, offset);
            break;
            case RT_CTL_HELLO_TEST:
                    do_hello_test_msg(
                        tvb, pinfo, ctl_msg_tree, offset);
            break;
            case RT_CTL_LVL1_ROUTING:
            case RT_CTL_LVL2_ROUTING:
                    do_routing_msg(
                        tvb, pinfo, ctl_msg_tree, offset, msg_flags >> 1);
            break;
            case RT_CTL_ETH_ROUTER_HELLO_MSG:
            case RT_CTL_ETH_ENDNODE_HELLO_MSG:
                    do_hello_msg(
                        tvb, pinfo, ctl_msg_tree, offset, msg_flags >> 1);
            break;
            default:
            break;
        }
    } else if (msg_flags & RT_FLAGS_LONG_MSG){
        proto_tree_add_uint(flags_tree, hf_dec_rt_long_msg,
                tvb, offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_rqr, tvb,
                    offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_rts, tvb,
                    offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_inter_eth, tvb,
                    offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_discard, tvb,
                    offset, 1, msg_flags);

        /* Increment offset by three:
                1 to get past the flags field
                2 to skip the DEC area/subarea field
         */
        offset += 3;
        ti = proto_tree_add_item(rt_tree, hf_dec_rt_dst_addr, tvb,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                offset, 6, ENC_NA);
        addr = dnet_ntoa((const guint8 *)tvb_memdup(wmem_packet_scope(), tvb, offset, 6));
        if (addr != NULL) {
            proto_item_append_text(ti, " (%s)", addr);
        }

        /* Skip 6 bytes for the MAC and
                2 bytes for DEC area/subarea
            */
        offset += 8;
        ti = proto_tree_add_item(rt_tree, hf_dec_rt_src_addr, tvb,
            offset, 6, ENC_NA);
        addr = dnet_ntoa((const guint8 *)tvb_memdup(wmem_packet_scope(), tvb, offset, 6));
        if (addr != NULL) {
            proto_item_append_text(ti, " (%s)", addr);
        }

        /* Proceed to the NL2 byte */
        offset += 6;
        proto_tree_add_uint(rt_tree, hf_dec_rt_nl2, tvb,
            offset, 1, rt_zero);
        offset++;
        rt_visit_count = tvb_get_guint8(tvb, offset);
        proto_tree_add_uint(rt_tree, hf_dec_rt_visit_count, tvb,
            offset, 1, rt_visit_count);
        offset++;
        proto_tree_add_uint(rt_tree, hf_dec_rt_service_class, tvb,
            offset, 1, rt_zero);
        offset++;
        proto_tree_add_uint(rt_tree, hf_dec_rt_protocol_type, tvb,
            offset, 1, rt_zero);
        offset++;
    } else {
        proto_tree_add_uint(flags_tree, hf_dec_rt_short_msg,
                tvb, offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_rqr, tvb,
                    offset, 1, msg_flags);
        proto_tree_add_boolean(flags_tree, hf_dec_rt_rts, tvb,
                    offset, 1, msg_flags);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        /* Increment offset to get past the flags field
         */
        offset++;
        proto_tree_add_item(rt_tree, hf_dec_rt_dst_node, tvb, offset, 2, ENC_LITTLE_ENDIAN);
        offset += 2;
        proto_tree_add_item(rt_tree, hf_dec_rt_src_node, tvb, offset, 2, ENC_LITTLE_ENDIAN);
        offset += 2;
        forward = tvb_get_guint8(tvb, offset);
        proto_tree_add_uint(rt_tree, hf_dec_rt_visited_nodes, tvb,
            offset, 1, forward);
        offset++;
    }

    if (!(msg_flags & RT_FLAGS_CTRL_MSG)) {
        /* It is not a routing control message */
        proto_tree *nsp_msg_tree;
        proto_item *ti_local;
        guint8     nsp_msg_type;

        nsp_msg_type = tvb_get_guint8(tvb, offset);
           ti_local = proto_tree_add_uint(
            tree, hf_dec_nsp_msgs, tvb, offset, 1, nsp_msg_type);
        if (nsp_msg_type == NOP_MSG) {
            /* Only test data in this msg */
            return;
        }
        nsp_msg_tree = proto_item_add_subtree(ti_local, ett_dec_rt_nsp_msg);
        /* Get past the nsp_msg_type */
        offset++;
        proto_tree_add_item(nsp_msg_tree, hf_dec_rt_dst_node, tvb, offset, 2, ENC_LITTLE_ENDIAN);
        offset += 2;
        if (nsp_msg_type == CONN_ACK_MSG) {
            col_set_str(pinfo->cinfo, COL_INFO, "NSP connect acknowledgement");
            /* Done with this msg type */
            return;
        }
        /* All other messages have a source node */
        proto_tree_add_item(nsp_msg_tree, hf_dec_rt_src_node, tvb, offset, 2, ENC_LITTLE_ENDIAN);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        offset += 2;

        handle_nsp_msg(tvb,
                           pinfo,
                           nsp_msg_tree,
                           offset,
                           nsp_msg_type);
    }
}
