static guint
CVE_2014_6424_PATCHED_dissect_v9_v10_pdu_data(tvbuff_t *tvb, packet_info *pinfo, proto_tree *pdutree, int offset,
                        v9_v10_tmplt_t *tmplt_p, hdrinfo_t *hdrinfo_p, v9_v10_tmplt_fields_type_t fields_type)
{
    int                   orig_offset;
    int                   rev;
    nstime_t              ts_start[2], ts_end[2];
    int                   offset_s[2], offset_e[2];
    nstime_t              ts;
    guint32               msec_start[2], msec_end[2];
    guint32               msec_delta;
    nstime_t              ts_delta;
    guint32               usec;
    int                   i;

    address               local_addr, remote_addr;
    guint16               local_port = 0, remote_port = 0/*, ipv4_id = 0, icmp_id = 0*/;
    guint32               uid = 0, pid = 0;
    int                   uname_len;
    gchar                *uname_str = NULL;
    int                   cmd_len;
    gchar                *cmd_str = NULL;
    guint16               got_flags = 0;

    int                   string_len_short = 0;
    int                   string_len_long = 0;

    proto_tree           *string_tree;

    gchar                *gen_str = NULL;
    int                   gen_str_offset = 0;

    proto_item           *ti;
    guint16               count;
    v9_v10_tmplt_entry_t *entries_p;
    proto_tree           *fwdstattree;

    entries_p = tmplt_p->fields_p[fields_type];
    if (entries_p == NULL) {
        /* I don't think we can actually hit this condition.
           If we can, what would cause it?  Does this need a
           warn?  If so, what?
        */
        return 0;
    }
    orig_offset   = offset;
    count         = tmplt_p->field_count[fields_type];

    offset_s[0]   = offset_s[1] = offset_e[0] = offset_e[1] = 0;
    msec_start[0] = msec_start[1] = msec_end[0] = msec_end[1] = 0;

    for (i = 0; i < count; i++) {
        guint64      pen_type;
        guint16      type;
        guint16      masked_type;
        guint16      length;
        guint32      pen;
        const gchar *pen_str;
        int          vstr_len;

        type    = entries_p[i].type;
        length  = entries_p[i].length;
        pen     = entries_p[i].pen;
        pen_str = entries_p[i].pen_str;

        if (length == 0) { /* XXX: Zero length fields probably shouldn't be included in the cached template */
            /* YYY: Maybe.  If you don't cache the zero length fields can you still compare that you actually */
            /* have the same template with the same ID.  See WMeier comment "c." above */
            continue;
        }
        /* See if variable length field */
        vstr_len = 0;
        if (length == VARIABLE_LENGTH) {
            vstr_len = 1;
            string_len_short = length = tvb_get_guint8(tvb, offset);
            if (length == 255) {
                vstr_len = 3;
                string_len_long = length = tvb_get_ntohs(tvb, offset+1);
            }
            offset += vstr_len;
            gen_str_offset = offset;
        }

        /*  v9 types
         *    0x 0000 0000 0000 to
         *    0x 0000 0000 ffff
         *  v10 global types (presumably consistent with v9 types 0x0000 - 0x7fff)
         *    0x 0000 0000 0000 to
         *    0x 0000 0000 7fff
         *  V10 Enterprise types
         *    0x 0000 0001 0000 to
         *    0x ffff ffff 7fff
         */
        pen_type = masked_type = type;
        rev      = 0;

        if ((hdrinfo_p->vspec == 10) && (type & 0x8000)) {
            pen_type = masked_type = type & 0x7fff;
            if (pen == REVPEN) { /* reverse PEN */
                rev = 1;
            } else if (pen == 0) {
                pen_type = (G_GUINT64_CONSTANT(0xffff) << 16) | pen_type;  /* hack to force "unknown" */
            } else {
                pen_type = (((guint64)pen) << 16) | pen_type;
            }
        }

        ti = NULL;
        switch (pen_type) {

        case 1: /* bytes */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Octets: length %u", length);
            }
            break;

        case 2: /* packets */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Packets: length %u", length);
            }
            break;

        case 163: /*  observedFlowTotalCount */
        case 3: /* flows */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_flows,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_flows64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Flows: length %u", length);
            }
            break;

        case 4: /* proto */
            ti = proto_tree_add_item(pdutree, hf_cflow_prot,
                                     tvb, offset, length, ENC_NA);
            break;

        case 5: /* TOS */
            ti = proto_tree_add_item(pdutree, hf_cflow_tos,
                                     tvb, offset, length, ENC_NA);
            break;

        case 6: /* TCP flags */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcpflags,
                                     tvb, offset, length, ENC_NA);
            break;

        case 7: /* source port */
        case 180: /*  udpSourcePort */
        case 182: /*  tcpSourcePort */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcport,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 8: /* source IP */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_srcaddr,
                                         tvb, offset, length, ENC_NA);
            } else if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_srcaddr_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "SrcAddr: length %u", length);
            }
            break;

        case 9: /* source mask */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcmask,
                                     tvb, offset, length, ENC_NA);
            break;

        case 10: /* input SNMP */
            ti = proto_tree_add_item(pdutree, hf_cflow_inputint,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 11: /* dest port */
        case 181: /*  udpDestinationPort */
        case 183: /*  tcpDestinationPort */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstport,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 12: /* dest IP */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_dstaddr,
                                         tvb, offset, length, ENC_NA);
            } else if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_dstaddr_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "DstAddr: length %u", length);
            }
            break;

        case 13: /* dest mask */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstmask,
                                     tvb, offset, length, ENC_NA);
            break;

        case 14: /* output SNMP */
            ti = proto_tree_add_item(pdutree, hf_cflow_outputint,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 15: /* nexthop IP */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_nexthop,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "NextHop: length %u", length);
            }
            break;

        case 16: /* source AS */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcas,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 17: /* dest AS */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstas,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 18: /* BGP nexthop IP */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_bgpnexthop,
                                         tvb, offset, length, ENC_NA);
            } else if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_bgpnexthop_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "BGPNextHop: length %u", length);
            }
            break;

        case 19: /* multicast packets */
            ti = proto_tree_add_item(pdutree, hf_cflow_mulpackets,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 20: /* multicast octets */
            ti = proto_tree_add_item(pdutree, hf_cflow_muloctets,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 21: /* last switched */
            offset_e[rev] = offset;
            msec_end[rev] = tvb_get_ntohl(tvb, offset);
            ts_end[rev].secs = msec_end[rev] / 1000;
            ts_end[rev].nsecs = (msec_end[rev] % 1000) * 1000000;
            goto timestamp_common;
            break;
        case 22: /* first switched */
            offset_s[rev] = offset;
            msec_start[rev] = tvb_get_ntohl(tvb, offset);
            ts_start[rev].secs = msec_start[rev] / 1000;
            ts_start[rev].nsecs = (msec_start[rev] % 1000) * 1000000;
            goto timestamp_common;
            break;

        case 150: /*  flowStartSeconds */
            offset_s[rev] = offset;
            ts_start[rev].secs = tvb_get_ntohl(tvb, offset);
            ts_start[rev].nsecs = 0;
            goto timestamp_common;
            break;

        case 151: /*  flowEndSeconds */
            offset_e[rev] = offset;
            ts_end[rev].secs = tvb_get_ntohl(tvb, offset);
            ts_end[rev].nsecs = 0;
            goto timestamp_common;
            break;

        case 152: /*  flowStartMilliseconds: 64-bit integer */
            offset_s[rev] = offset;
            ts_start[rev].secs = (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts_start[rev].nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) * 1000000;
            goto timestamp_common;
            break;

        case 153: /*  flowEndMilliseconds; 64-bit integer */
            offset_e[rev] = offset;
            ts_end[rev].secs  = (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts_end[rev].nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) * 1000000;
            goto timestamp_common;
            break;

        case 154: /*  flowStartMicroseconds: 64-bit NTP format */
            offset_s[rev] = offset;
            ntp_to_nstime(tvb, offset, &ts_start[rev]);
            goto timestamp_common;
            break;

        case 155: /*  flowEndMicroseconds: 64-bit NTP format */
            /*  XXX: Not tested ...                    */
            offset_e[rev] = offset;
            ntp_to_nstime(tvb, offset, &ts_end[rev]);
            goto timestamp_common;
            break;

        case 156: /*  flowStartNanoseconds: 64-bit NTP format */
            /*  XXX: Not tested ...                     */
            offset_s[rev] = offset;
            ntp_to_nstime(tvb, offset, &ts_start[rev]);
            goto timestamp_common;
            break;

        case 157: /*  flowEndNanoseconds: 64-bit NTP format */
            /*  XXX: Not tested ...                   */
            offset_e[rev] = offset;
            ntp_to_nstime(tvb, offset, &ts_end[rev]);
            goto timestamp_common;
            break;

        case 158: /*  flowStartDeltaMicroseconds: 32-bit integer; negative time offset   */
            /*   relative to the export time specified in the IPFIX Message Header */
            /*  XXX: Not tested ...                                                */
            offset_s[rev]       = offset;
            usec                = tvb_get_ntohl(tvb, offset);
            ts_start[rev].secs  = (time_t)(((guint64)(hdrinfo_p->export_time_secs)*1000000 - usec) / 1000000);
            ts_start[rev].nsecs = (int)(((guint64)(hdrinfo_p->export_time_secs)*1000000 - usec) % 1000000) * 1000;
            goto timestamp_common;
            break;

        case 159: /*  flowEndDeltaMicroseconds: 32-bit integer; negative time offset     */
            /*   relative to the export time specified in the IPFIX Message Header */
            /*  XXX: Not tested ...                                                */
            offset_e[rev] = offset;
            usec          = tvb_get_ntohl(tvb, offset);
            ts_end[rev].secs  = (time_t)(((guint64)(hdrinfo_p->export_time_secs)*1000000 - usec) / 1000000);
            ts_end[rev].nsecs = (int)(((guint64)(hdrinfo_p->export_time_secs)*1000000 - usec) % 1000000) * 1000;

            /* This code executed for all timestamp fields above  */
            /* !! Assumption: Only 1 set of time fields in a flow */
        timestamp_common:
            if(offset_s[rev] && offset_e[rev]) {
                proto_tree *timetree;
                proto_item *timeitem;

                nstime_delta(&ts_delta, &ts_end[rev], &ts_start[rev]);
                timeitem =
                    proto_tree_add_time(pdutree, hf_cflow_timedelta, tvb,
                                        offset_s[rev], 0, &ts_delta);
                PROTO_ITEM_SET_GENERATED(timeitem);
                timetree = proto_item_add_subtree(timeitem, ett_flowtime);
                /* Note: length of "start" is assumed to match that of "end" */
                if (msec_start[rev]) {
                    proto_tree_add_time(timetree, hf_cflow_timestart, tvb,
                                        offset_s[rev], length, &ts_start[rev]);
                } else {
                    proto_tree_add_time(timetree, hf_cflow_abstimestart, tvb,
                                        offset_s[rev], length, &ts_start[rev]);
                }
                if (msec_end[rev]) {
                    proto_tree_add_time(timetree, hf_cflow_timeend, tvb,
                                        offset_e[rev], length, &ts_end[rev]);
                } else {
                    proto_tree_add_time(timetree, hf_cflow_abstimeend, tvb,
                                        offset_e[rev], length, &ts_end[rev]);
                }
            }
            break;

        case 23: /* postOctetDeltaCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Octets: length %u", length);
            }
            break;

        case 24: /* postPacketDeltaCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Packets: length %u", length);
            }
            break;

        case 25: /* length_min */
            if (length == 2) {
                ti = proto_tree_add_item(pdutree, hf_cflow_length_min,
                        tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_length_min64,
                        tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "MinLength: length %u", length);
            }
            break;

        case 26: /* length_max */
            if (length == 2) {
                ti = proto_tree_add_item(pdutree, hf_cflow_length_max,
                        tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_length_max64,
                        tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "MaxLength: length %u", length);
            }
            break;

        case 27: /* IPv6 src addr */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcaddr_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 28: /* IPv6 dst addr */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstaddr_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 29: /* IPv6 src addr mask */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcmask_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 30: /* IPv6 dst addr mask */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstmask_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 31: /* flowLabelIPv6 */
            /*  RFC5102 defines that Abstract Data Type of this
                Information Element is unsigned32 */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ipv6_flowlabel,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            /* RFC3954 defines that length of this field is 3
               Bytes */
            else if (length == 3) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ipv6_flowlabel24,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;

        case 32: /* ICMP_TYPE */
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 33: /* IGMP_TYPE */
            ti = proto_tree_add_item(pdutree, hf_cflow_igmp_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 34: /* sampling interval */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_interval,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 35: /* sampling algorithm */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_algorithm,
                                     tvb, offset, length, ENC_NA);
            break;

        case 36: /* flow active timeout */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_active_timeout,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 37: /* flow inactive timeout */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_inactive_timeout,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 38: /* engine type */
            ti = proto_tree_add_item(pdutree, hf_cflow_engine_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 39: /* engine id*/
            ti = proto_tree_add_item(pdutree, hf_cflow_engine_id,
                                     tvb, offset, length, ENC_NA);
            break;

        case 40: /* bytes exported */
            if( length == 8 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_octets_exp64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if( length == 4 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_octets_exp,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "BytesExported: length %u", length);
            }
            break;

        case 41: /* packets exported */
            if( length == 8 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_packets_exp64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if( length == 4 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_packets_exp,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "PacketsExported: length %u", length);
            }
            break;

        case 42: /* flows exported */
            if( length == 8 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_flows_exp64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if( length == 4 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_flows_exp,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "FlowsExported: length %u", length);
            }
            break;

        case 44: /* IP source prefix */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_srcprefix,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "SrcPrefix: length %u", length);
            }
            break;

        case 45: /* IP destination prefix */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_dstprefix,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "DstPrefix: length %u", length);
            }
            break;

        case 46: /* top MPLS label type*/
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_top_label_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 47: /* top MPLS label PE address*/
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_pe_addr,
                                     tvb, offset, length, ENC_NA);
            break;

        case 48: /* Flow Sampler ID */
#if 0
            /* XXX: Why was code originally add_text for just this one case ? */
            ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                     "FlowSamplerID: %d", tvb_get_guint8(tvb, offset));
#endif
            ti = proto_tree_add_item(pdutree, hf_cflow_sampler_id,
                                     tvb, offset, length, ENC_NA);
            break;

        case 49: /* FLOW_SAMPLER_MODE  */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampler_mode,
                                     tvb, offset, length, ENC_NA);
            break;

        case 50: /* FLOW_SAMPLER_RANDOM_INTERVAL  */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampler_random_interval,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 51: /*  FLOW_CLASS */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_class,
                                     tvb, offset, length, ENC_NA);
            break;

        case 52: /*  TTL_MINIMUM */
            ti = proto_tree_add_item(pdutree, hf_cflow_ttl_minimum,
                                     tvb, offset, length, ENC_NA);
            break;

        case 53: /*  TTL_MAXIMUM */
            ti = proto_tree_add_item(pdutree, hf_cflow_ttl_maximum,
                                     tvb, offset, length, ENC_NA);
            break;

        case 54: /* IPV4_ID  */
            ti = proto_tree_add_item(pdutree, hf_cflow_ipv4_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 55: /* postIpClassOfService */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_tos,
                                     tvb, offset, length, ENC_NA);
            break;

        case 56: /* sourceMacAddress */
            ti = proto_tree_add_item(pdutree, hf_cflow_srcmac,
                                     tvb, offset, length, ENC_NA);
            break;

        case 57: /* postDestinationMacAddress */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_dstmac,
                                     tvb, offset, length, ENC_NA);
            break;

        case 58: /* vlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_vlanid,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 59: /* postVlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_vlanid,
                                     tvb, offset, length, ENC_NA);
            break;

        case 60: /* IP_VERSION */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_version,
                                     tvb, offset, length, ENC_NA);
            break;

        case 61: /* DIRECTION   */
            ti = proto_tree_add_item(pdutree, hf_cflow_direction,
                                     tvb, offset, length, ENC_NA);
            break;

        case 62: /* IPV6_NEXT_HOP */
            if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_nexthop_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "NextHop: length %u", length);
            }
            break;

        case 63: /* BGP_IPV6_NEXT_HOP */
            if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_bgpnexthop_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "BGPNextHop: length %u", length);
            }
            break;

        case 64: /* ipv6ExtensionHeaders */
            ti = proto_tree_add_item(pdutree, hf_cflow_ipv6_exthdr,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 70: /* MPLS label1*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 1);
            break;

        case 71: /* MPLS label2*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 2);
            break;

        case 72: /* MPLS label3*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 3);
            break;

        case 73: /* MPLS label4*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 4);
            break;

        case 74: /* MPLS label5*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 5);
            break;

        case 75: /* MPLS label6*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 6);
            break;

        case 76: /* MPLS label7*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 7);
            break;

        case 77: /* MPLS label8*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 8);
            break;

        case 78: /* MPLS label9*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 9);
            break;

        case 79: /* MPLS label10*/
            ti = proto_tree_add_mpls_label(pdutree, tvb, offset, length, 10);
            break;

        case 80: /* destinationMacAddress */
            ti = proto_tree_add_item(pdutree, hf_cflow_dstmac,
                                     tvb, offset, length, ENC_NA);
            break;

        case 81: /* postSourceMacAddress */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_srcmac,
                                     tvb, offset, length, ENC_NA);
            break;

        case 82: /* IF_NAME  */
            ti = proto_tree_add_item(pdutree, hf_cflow_if_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 83: /* IF_DESCR  */
            ti = proto_tree_add_item(pdutree, hf_cflow_if_descr,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 84: /* SAMPLER_NAME  */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampler_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 85: /* BYTES_PERMANENT */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_permanent_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_permanent_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Running Octets: length %u", length);
            }
            break;

        case 86: /* PACKETS_PERMANENT */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_permanent_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_permanent_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Running Packets: length %u", length);
            }
            break;

        case 88: /* fragmentOffset */
            ti = proto_tree_add_item(pdutree, hf_cflow_fragment_offset,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 89: {
            /* FORWARDING_STATUS */
            /* Forwarding status is encoded on 1 byte with
             * the 2 left bits giving the status and the 6
             * remaining bits giving the reason code. */

            guint8              forwarding_status;
            const value_string *x_vs;
            int                 x_hf;

            ti = proto_tree_add_text(pdutree, tvb, offset, length, "Forwarding Status");
            fwdstattree = proto_item_add_subtree(ti, ett_fwdstat);

            forwarding_status = tvb_get_guint8(tvb, offset)>>6;
            switch(forwarding_status) {
            default:
            case FORWARDING_STATUS_UNKNOWN:
                x_vs = v9_forwarding_status_unknown_code;
                x_hf = hf_cflow_forwarding_status_unknown_code;
                break;
            case FORWARDING_STATUS_FORWARD:
                x_vs = v9_forwarding_status_forward_code;
                x_hf = hf_cflow_forwarding_status_forward_code;
                break;
            case FORWARDING_STATUS_DROP:
                x_vs = v9_forwarding_status_drop_code;
                x_hf = hf_cflow_forwarding_status_drop_code;
                break;
            case FORWARDING_STATUS_CONSUME:
                x_vs = v9_forwarding_status_consume_code;
                x_hf = hf_cflow_forwarding_status_consume_code;
                break;
            }

            proto_tree_add_item(fwdstattree, hf_cflow_forwarding_status,
                                tvb, offset, length, ENC_NA);

            proto_tree_add_item(fwdstattree, x_hf,
                                tvb, offset, length, ENC_NA);

            /* add status code to tree summary */
            if (length==1) {
                proto_item_append_text(ti, ": %s", val_to_str_const(forwarding_status,
                                                                    v9_forwarding_status, "(Unknown)"));
                proto_item_append_text(ti, ": %s", val_to_str_const((tvb_get_guint8(tvb, offset)&0x3F),
                                                                    x_vs, "(Unknown)"));
            };
        }
            break;

        case 90: /* mplsVpnRouteDistinguisher */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_vpn_rd,
                                     tvb, offset, length, ENC_NA);
            break;

        case 91: /* mplsTopLabelPrefixLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_top_label_prefix_length,
                                     tvb, offset, length, ENC_NA);
            break;

        case 94: /* NBAR applicationDesc */
            ti = proto_tree_add_item(pdutree, hf_cflow_nbar_appl_desc,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 95: /* NBAR applicationId */
            ti = proto_tree_add_item(pdutree, hf_cflow_nbar_appl_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 96: /* NBAR applicationName */
            ti = proto_tree_add_item(pdutree, hf_cflow_nbar_appl_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 98: /* postIpDiffServCodePoint */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_ip_diff_serv_code_point,
                                     tvb, offset, length, ENC_NA);
            break;

        case 99: /* multicastReplicationFactor */
            ti = proto_tree_add_item(pdutree, hf_cflow_multicast_replication_factor,
                                     tvb, offset, length, ENC_NA);
            break;

        case 128: /* dest AS Peer */
            ti = proto_tree_add_item(pdutree, hf_cflow_peer_dstas,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 129: /* source AS Peer*/
            ti = proto_tree_add_item(pdutree, hf_cflow_peer_srcas,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 130: /*  exporterIPv4Address */
            ti = proto_tree_add_item(pdutree, hf_cflow_exporter_addr,
                                     tvb, offset, length, ENC_NA);
            break;

        case 131: /*  exporterIPv6Address */
            ti = proto_tree_add_item(pdutree,
                                     hf_cflow_exporter_addr_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 132: /*  droppedOctetDeltaCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "Dropped Octets: length %u",
                                         length);
            }
            break;

        case 133: /*  droppedPacketDeltaCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "Dropped Packets: length %u",
                                         length);
            }
            break;

        case 134: /*  droppedOctetTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_total_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_total_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "Dropped Total Octets: length %u", length);
            }
            break;

        case 135: /*  droppedPacketTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_total_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_drop_total_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "Dropped Total Packets: length %u", length);
            }
            break;

        case 136: /*  flowEndReason */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_end_reason,
                                     tvb, offset, length, ENC_NA);
            break;

        case 137: /*  commonPropertiesId */
            ti = proto_tree_add_item(pdutree, hf_cflow_common_properties_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 138: /*  observationPointId */
            ti = proto_tree_add_item(pdutree, hf_cflow_observation_point_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 139: /* icmpTypeCodeIPv6 */
            proto_tree_add_item(pdutree, hf_cflow_icmp_ipv6_type,
                                     tvb, offset, 1, ENC_NA);
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_ipv6_code,
                                     tvb, offset + 1, 1, ENC_NA);
            break;

        case 140: /*  mplsTopLabelIPv6Address */
            if (length == 16) {
                ti = proto_tree_add_item(pdutree,
                                         hf_cflow_mpls_pe_addr_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "mplsTopLabelIPv6Addr: length %u",
                                         length);
            }
            break;

        case 141: /*  lineCardId */
            ti = proto_tree_add_item(pdutree, hf_cflow_scope_linecard,
                                     tvb, offset, length, ENC_NA);
            break;

        case 142: /*  portId */
            ti = proto_tree_add_item(pdutree, hf_cflow_port_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 143: /*  meteringProcessId */
            ti = proto_tree_add_item(pdutree, hf_cflow_mp_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 144: /* FLOW EXPORTER */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_exporter,
                                     tvb, offset, length, ENC_NA);
            break;

        case 145: /*  templateId */
            ti = proto_tree_add_item(pdutree, hf_cflow_template_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 146: /*  wlanChannelId */
            ti = proto_tree_add_item(pdutree, hf_cflow_wlan_channel_id,
                                     tvb, offset, length, ENC_NA);
            break;

        case 147: /*  wlanSSID */
            ti = proto_tree_add_item(pdutree, hf_cflow_wlan_ssid,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 148: /*  flowId */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 149: /*  observationDomainId */
            ti = proto_tree_add_item(pdutree, hf_cflow_od_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 160: /*  systemInitTimeMilliseconds */
            ts.secs  = (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts.nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) *1000000;
            ti = proto_tree_add_time(pdutree,
                                     hf_cflow_sys_init_time,
                                     tvb, offset, length, &ts);
            break;

        case 161: /*  flowDurationMilliseconds */
            msec_delta = tvb_get_ntohl(tvb, offset);
            ts_delta.secs = msec_delta / 1000;
            ts_delta.nsecs = (msec_delta % 1000) * 1000000;
            ti = proto_tree_add_time(pdutree, hf_cflow_timedelta, tvb,
                                     offset, length, &ts_delta);
            break;

        case 162: /*  flowDurationMicroseconds */
            msec_delta = tvb_get_ntohl(tvb, offset);
            ts_delta.secs = msec_delta / 1000000;
            ts_delta.nsecs = (msec_delta % 1000000) * 1000;
            ti = proto_tree_add_time(pdutree, hf_cflow_timedelta, tvb,
                                     offset, length, &ts_delta);
            break;

        case 164: /*  ignoredPacketTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ignore_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ignore_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Ignored Packets: length %u", length);
            }
            break;

        case 165: /*  ignoredOctetTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ignore_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_ignore_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Ignored Octets: length %u", length);
            }
            break;

        case 166: /*  notSentFlowTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_flows,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_flows64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Not Sent Flows: length %u", length);
            }
            break;

        case 167: /*  notSentPacketTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Not Sent Packets: length %u", length);
            }
            break;

        case 168: /*  notSentOctetTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_notsent_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Not Sent Packets: length %u", length);
            }
            break;

        case 169: /* destinationIPv6Prefix */
            if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_dstnet_v6,
                                         tvb, offset, length, ENC_NA);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "DstPrefix: length %u", length);
            }
            break;

        case 170: /* sourceIPv6Prefix */
            if (length == 16) {
                ti = proto_tree_add_item(pdutree, hf_cflow_srcnet_v6,
                                         tvb, offset, length, ENC_NA);
            } else if (length != 4 && length != 16) {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "SrcPrefix: length %u", length);
            }
            break;

        case 171: /* postOctetTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_octets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_octets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Total Octets: length %u", length);
            }
            break;

        case 172: /* postPacketTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_packets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_packets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Total Packets: length %u", length);
            }
            break;

        case 173: /* flowKeyIndicator */
            ti = proto_tree_add_item(pdutree, hf_cflow_key,
                                     tvb, offset, length, ENC_NA);
            break;

        case 174: /* postMCastPacketTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_mulpackets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_mulpackets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Total Multicast Packets: length %u", length);
            }
            break;

        case 175: /* postMCastOctetTotalCount */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_muloctets,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else if (length == 8) {
                ti = proto_tree_add_item(pdutree, hf_cflow_post_total_muloctets64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "Post Total Multicast Octets: length %u", length);
            }
            break;

        case 176: /* ICMP_IPv4_TYPE */
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_ipv4_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 177: /* ICMP_IPv4_CODE */
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_ipv4_code,
                                     tvb, offset, length, ENC_NA);
            break;

        case 178: /* ICMP_IPv6_TYPE */
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_ipv6_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 179: /* ICMP_IPv6_CODE */
            ti = proto_tree_add_item(pdutree, hf_cflow_icmp_ipv6_code,
                                     tvb, offset, length, ENC_NA);
            break;

        case 184: /* tcpSequenceNumber */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_seq_num,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 185: /* tcpAcknowledgementNumber */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_ack_num,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 186: /* TCP_WINDOWS_SIZE */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_window_size,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
            break;

        case 187: /* tcpUrgentPointer */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_urg_ptr,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 188: /* tcpHeaderLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_header_length,
                                     tvb, offset, length, ENC_NA);
            break;

        case 189: /* ipHeaderLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_header_length,
                                     tvb, offset, length, ENC_NA);
            break;

        case 190: /* IPV4_TOTAL_LENGTH */
            ti = proto_tree_add_item(pdutree, hf_cflow_ipv4_total_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 191: /* payloadLengthIPv6 */
            ti = proto_tree_add_item(pdutree, hf_cflow_ipv6_payload_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 192: /* IP_TTL */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_ttl,
                                     tvb, offset, length, ENC_NA);
            break;

        case 193: /* nextHeaderIPv6 */
            ti = proto_tree_add_item(pdutree, hf_cflow_ipv6_next_hdr,
                                     tvb, offset, length, ENC_NA);
            break;

        case 194: /* IP_TOS */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_tos,
                                     tvb, offset, length, ENC_NA);
            break;

        case 195: /* IP_DSCP */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_dscp,
                                     tvb, offset, length, ENC_NA);
            break;

        case 196: /* ipPrecedence */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_precedence,
                                     tvb, offset, length, ENC_NA);
            break;

        case 197: /* fragmentFlags */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_fragment_flags,
                                     tvb, offset, length, ENC_NA);
            break;

        case 198: /* BYTES_SQUARED */
        case 199: /* BYTES_SQUARED_PERMANENT */
            if( length == 8 ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_octets_squared64,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                         "Bytes Squared: length %u", length);
            }
            break;
        case 200: /* mplsTopLabelTTL */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_top_label_ttl,
                                     tvb, offset, length, ENC_NA);
            break;

        case 201: /* mplsLabelStackLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_label_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 202: /* mplsLabelStackDepth */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_label_depth,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 203: /* mplsTopLabelExp */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_top_label_exp,
                                     tvb, offset, length, ENC_NA);
            break;

        case 204: /* ipPayloadLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_payload_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 205: /* UDP_LENGTH */
            ti = proto_tree_add_item(pdutree, hf_cflow_udp_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 206: /* IS_MULTICAST */
            ti = proto_tree_add_item(pdutree, hf_cflow_is_multicast,
                                     tvb, offset, length, ENC_NA);
            break;

        case 207: /* IP_HEADER_WORDS */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_header_words,
                                     tvb, offset, length, ENC_NA);
            break;

        case 208: /* OPTION_MAP */
            ti = proto_tree_add_item(pdutree, hf_cflow_option_map,
                                     tvb, offset, length, ENC_NA);
            break;

        case 209: /* tcpOptions */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_option_map,
                                     tvb, offset, length, ENC_NA);
            break;

        case 210: /* paddingOctets */
            ti = proto_tree_add_text(pdutree, tvb, offset, length,
                                     "Padding (%u byte%s)",
                                     length, plurality(length, "", "s"));
            break;

        case 211: /* collectorIPv4Address */
            ti = proto_tree_add_item(pdutree, hf_cflow_collector_addr,
                                     tvb, offset, length, ENC_NA);
            break;

        case 212: /* collectorIPv6Address */
            ti = proto_tree_add_item(pdutree, hf_cflow_collector_addr_v6,
                                     tvb, offset, length, ENC_NA);
            break;

        case 213: /* exportInterface */
            if (length == 4) {
                ti = proto_tree_add_item(pdutree, hf_cflow_export_interface,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_text(pdutree,
                                         tvb, offset, length,
                                         "exportInterface: invalid size %d", length );
            }
            break;

        case 214: /* exportProtocolVersion */
            ti = proto_tree_add_item(pdutree, hf_cflow_export_protocol_version,
                                     tvb, offset, length, ENC_NA);
            break;

        case 215: /* exportTransportProtocol */
            ti = proto_tree_add_item(pdutree, hf_cflow_export_prot,
                                     tvb, offset, length, ENC_NA);
            break;

        case 216: /* collectorTransportPort */
            ti = proto_tree_add_item(pdutree, hf_cflow_collector_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 217: /* exporterTransportPort */
            ti = proto_tree_add_item(pdutree, hf_cflow_exporter_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 218: /* tcpSynTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_syn,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 219: /* tcpFinTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_fin,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 220: /* tcpRstTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_rst,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 221: /* tcpPshTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_psh,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 222: /* tcpAckTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_ack,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 223: /* tcpUrgTotalCount */
            ti = proto_tree_add_item(pdutree, hf_cflow_total_tcp_urg,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 224: /* IP_TOTAL_LENGTH */
            ti = proto_tree_add_item(pdutree, hf_cflow_ip_total_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 225: /* postNATSourceIPv4Address */
        case 40001: /* NF_F_XLATE_SRC_ADDR_IPV4 (Cisco ASA 5500 Series) */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_natsource_ipv4_address,
                                     tvb, offset, length, ENC_NA);
            break;

        case 226: /* postNATDestinationIPv4Address */
        case 40002: /* NF_F_XLATE_DST_ADDR_IPV4 (Cisco ASA 5500 Series) */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_natdestination_ipv4_address,
                                     tvb, offset, length, ENC_NA);
            break;

        case 227: /* postNAPTSourceTransportPort */
        case 40003: /* NF_F_XLATE_SRC_PORT (Cisco ASA 5500 Series) */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_naptsource_transport_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 228: /* postNAPTDestinationTransportPort */
        case 40004: /* NF_F_XLATE_DST_PORT (Cisco ASA 5500 Series) */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_naptdestination_transport_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 229: /* natOriginatingAddressRealm */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_originating_address_realm,
                                     tvb, offset, length, ENC_NA);
            break;

        case 230: /* natEvent */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_event,
                                     tvb, offset, length, ENC_NA);
            break;

        case 231: /* initiatorOctets */
            ti = proto_tree_add_item(pdutree, hf_cflow_initiator_octets,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 232: /* responderOctets */
            ti = proto_tree_add_item(pdutree, hf_cflow_responder_octets,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 233: /* firewallEvent */
        case 40005: /* NF_F_FW_EVENT (Cisco ASA 5500 Series) */
            ti = proto_tree_add_item(pdutree, hf_cflow_firewall_event,
                                     tvb, offset, length, ENC_NA);
            break;

        case 234: /* ingressVRFID */
            ti = proto_tree_add_item(pdutree, hf_cflow_ingress_vrfid,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 235: /* egressVRFID */
            ti = proto_tree_add_item(pdutree, hf_cflow_egress_vrfid,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 236: /* VRFname */
            ti = proto_tree_add_item(pdutree, hf_cflow_vrfname,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 237: /* postMplsTopLabelExp */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_mpls_top_label_exp,
                                     tvb, offset, length, ENC_NA);
            break;

        case 238: /* tcpWindowScale */
            ti = proto_tree_add_item(pdutree, hf_cflow_tcp_window_scale,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 239: /*  biflowDirection */
            ti = proto_tree_add_item(pdutree, hf_cflow_biflow_direction,
                                     tvb, offset, length, ENC_NA);
            break;

        case 240: /* ethernetHeaderLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_ethernet_header_length,
                                     tvb, offset, length, ENC_NA);
            break;

        case 241: /* ethernetPayloadLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_ethernet_payload_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 242: /* ethernetTotalLength */
            ti = proto_tree_add_item(pdutree, hf_cflow_ethernet_total_length,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 243: /* dot1qVlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_dot1q_vlan_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 244: /* dot1qPriority */
            ti = proto_tree_add_item(pdutree, hf_cflow_dot1q_priority,
                                     tvb, offset, length, ENC_NA);
            break;

        case 245: /* dot1qCustomerVlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_dot1q_customer_vlan_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 246: /* dot1qCustomerPriority */
            ti = proto_tree_add_item(pdutree, hf_cflow_dot1q_customer_priority,
                                     tvb, offset, length, ENC_NA);
            break;

        case 247: /* metroEvcId */
            ti = proto_tree_add_item(pdutree, hf_cflow_metro_evc_id,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 248: /* metroEvcType */
            ti = proto_tree_add_item(pdutree, hf_cflow_metro_evc_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 249: /* pseudoWireId */
            ti = proto_tree_add_item(pdutree, hf_cflow_pseudo_wire_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 250: /* pseudoWireType */
            ti = proto_tree_add_item(pdutree, hf_cflow_pseudo_wire_type,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 251: /* pseudoWireControlWord */
            ti = proto_tree_add_item(pdutree, hf_cflow_pseudo_wire_control_word,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 252: /* ingressPhysicalInterface */
            ti = proto_tree_add_item(pdutree, hf_cflow_ingress_physical_interface,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 253: /* egressPhysicalInterface */
            ti = proto_tree_add_item(pdutree, hf_cflow_egress_physical_interface,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 254: /* postDot1qVlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_dot1q_vlan_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 255: /* postDot1qCustomerVlanId */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_dot1q_customer_vlan_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 256: /* ethernetType */
            ti = proto_tree_add_item(pdutree, hf_cflow_ethernet_type,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 257: /* postIpPrecedence */
            ti = proto_tree_add_item(pdutree, hf_cflow_post_ip_precedence,
                                     tvb, offset, length, ENC_NA);
            break;

        case 258: /* collectionTimeMilliseconds */
            ts.secs  = (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts.nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) *1000000;
            ti = proto_tree_add_time(pdutree,
                                     hf_cflow_collection_time_milliseconds,
                                     tvb, offset, length, &ts);
            break;

        case 259: /* exportSctpStreamId */
            ti = proto_tree_add_item(pdutree, hf_cflow_export_sctp_stream_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 260: /* maxExportSeconds */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_max_export_seconds,
                                     tvb, offset, length, &ts);
            break;

        case 261: /* maxFlowEndSeconds */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_max_flow_end_seconds,
                                     tvb, offset, length, &ts);
            break;

        case 262: /* messageMD5Checksum */
            ti = proto_tree_add_item(pdutree, hf_cflow_message_md5_checksum,
                                     tvb, offset, length, ENC_NA);
            break;

        case 263: /* messageScope */
            ti = proto_tree_add_item(pdutree, hf_cflow_message_scope,
                                     tvb, offset, length, ENC_NA);
            break;

        case 264: /* minExportSeconds */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_min_export_seconds,
                                     tvb, offset, length, &ts);
            break;

        case 265: /* minFlowStartSeconds */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_min_flow_start_seconds,
                                     tvb, offset, length, &ts);
            break;

        case 266: /* opaqueOctets */
            ti = proto_tree_add_item(pdutree, hf_cflow_opaque_octets,
                                     tvb, offset, length, ENC_NA);
            break;

        case 267: /* sessionScope */
            ti = proto_tree_add_item(pdutree, hf_cflow_session_scope,
                                     tvb, offset, length, ENC_NA);
            break;

        case 268: /* maxFlowEndMicroseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_max_flow_end_microseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 269: /* maxFlowEndMilliseconds */
            ts.secs =  (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts.nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) * 1000000;
            ti = proto_tree_add_time(pdutree, hf_cflow_max_flow_end_milliseconds,
                                     tvb, offset, length, &ts);
            break;

        case 270: /* maxFlowEndNanoseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_max_flow_end_nanoseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 271: /* minFlowStartMicroseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_min_flow_start_microseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 272: /* minFlowStartMilliseconds */
            ts.secs  = (tvb_get_ntohl(tvb, offset)/1000);
            ts.nsecs = (tvb_get_ntohl(tvb, offset)%1000) * 1000000;
            ti = proto_tree_add_time(pdutree, hf_cflow_min_flow_start_milliseconds,
                                     tvb, offset, length, &ts);
            break;

        case 273: /* minFlowStartNanoseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_min_flow_start_nanoseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 274: /* collectorCertificate */
            ti = proto_tree_add_item(pdutree, hf_cflow_collector_certificate,
                                     tvb, offset, length, ENC_NA);
            break;

        case 275: /* exporterCertificate */
            ti = proto_tree_add_item(pdutree, hf_cflow_exporter_certificate,
                                     tvb, offset, length, ENC_NA);
            break;

        case 301: /* selectionSequenceId */
            ti = proto_tree_add_item(pdutree, hf_cflow_selection_sequence_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 302: /* selectorId */
            ti = proto_tree_add_item(pdutree, hf_cflow_selector_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 303: /* informationElementId */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 304: /* selectorAlgorithm */
            ti = proto_tree_add_item(pdutree, hf_cflow_selector_algorithm,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 305: /* samplingPacketInterval */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_packet_interval,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 306: /* samplingPacketSpace */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_packet_space,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 307: /* samplingTimeInterval */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_time_interval,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 308: /* samplingTimeSpace */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_time_space,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 309: /* samplingSize */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_size,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 310: /* samplingPopulation */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_population,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 311: /* samplingProbability */
            ti = proto_tree_add_item(pdutree, hf_cflow_sampling_probability,
                                     tvb, offset, length, ENC_NA);
            break;

        case 313: /* SECTION_HEADER */
            ti = proto_tree_add_item(pdutree, hf_cflow_section_header,
                                     tvb, offset, length, ENC_NA);
            break;

        case 314: /* SECTION_PAYLOAD */
            ti = proto_tree_add_item(pdutree, hf_cflow_section_payload,
                                     tvb, offset, length, ENC_NA);
            break;

        case 316: /* mplsLabelStackSection */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_label_stack_section,
                                     tvb, offset, length, ENC_NA);
            break;

        case 317: /* mplsPayloadPacketSection */
            ti = proto_tree_add_item(pdutree, hf_cflow_mpls_payload_packet_section,
                                     tvb, offset, length, ENC_NA);
            break;

        case 318: /* selectorIdTotalPktsObserved */
            ti = proto_tree_add_item(pdutree, hf_cflow_selector_id_total_pkts_observed,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 319: /* selectorIdTotalPktsSelected */
            ti = proto_tree_add_item(pdutree, hf_cflow_selector_id_total_pkts_selected,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 320: /* absoluteError */
            ti = proto_tree_add_item(pdutree, hf_cflow_absolute_error,
                                     tvb, offset, length, ENC_NA);
            break;

        case 321: /* relativeError */
            ti = proto_tree_add_item(pdutree, hf_cflow_relative_error,
                                     tvb, offset, length, ENC_NA);
            break;

        case 322: /* observationTimeSeconds */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_observation_time_seconds,
                                     tvb, offset, length, &ts);
            break;

        case 323: /* observationTimeMilliseconds */
            ts.secs  = (time_t)(tvb_get_ntoh64(tvb, offset)/1000);
            ts.nsecs = (int)(tvb_get_ntoh64(tvb, offset)%1000) * 1000000;
            ti = proto_tree_add_time(pdutree, hf_cflow_observation_time_milliseconds,
                                     tvb, offset, length, &ts);
            break;

        case 324: /* observationTimeMicroseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_observation_time_microseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 325: /* observationTimeNanoseconds */
            ti = proto_tree_add_item(pdutree, hf_cflow_observation_time_nanoseconds,
                                     tvb, offset, length, ENC_TIME_NTP|ENC_BIG_ENDIAN);
            break;

        case 326: /* digestHashValue */
            ti = proto_tree_add_item(pdutree, hf_cflow_digest_hash_value,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 327: /* hashIPPayloadOffset */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_ippayload_offset,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 328: /* hashIPPayloadSize */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_ippayload_size,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 329: /* hashOutputRangeMin */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_output_range_min,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 330: /* hashOutputRangeMax */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_output_range_max,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 331: /* hashSelectedRangeMin */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_selected_range_min,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 332: /* hashSelectedRangeMax */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_selected_range_max,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 333: /* hashDigestOutput */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_digest_output,
                                     tvb, offset, length, ENC_NA);
            break;

        case 334: /* hashInitialiserValue */
            ti = proto_tree_add_item(pdutree, hf_cflow_hash_initialiser_value,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 335: /* selectorName */
            ti = proto_tree_add_item(pdutree, hf_cflow_selector_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 336: /* upperCILimit */
            ti = proto_tree_add_item(pdutree, hf_cflow_upper_cilimit,
                                     tvb, offset, length, ENC_NA);
            break;

        case 337: /* lowerCILimit */
            ti = proto_tree_add_item(pdutree, hf_cflow_lower_cilimit,
                                     tvb, offset, length, ENC_NA);
            break;

        case 338: /* confidenceLevel */
            ti = proto_tree_add_item(pdutree, hf_cflow_confidence_level,
                                     tvb, offset, length, ENC_NA);
            break;

        case 339: /* informationElementDataType */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_data_type,
                                     tvb, offset, length, ENC_NA);
            break;

        case 340: /* informationElementDescription */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_description,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 341: /* informationElementName */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 342: /* informationElementRangeBegin */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_range_begin,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 343: /* informationElementRangeEnd */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_range_end,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 344: /* informationElementSemantics */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_semantics,
                                     tvb, offset, length, ENC_NA);
            break;

        case 345: /* informationElementUnits */
            ti = proto_tree_add_item(pdutree, hf_cflow_information_element_units,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 346: /* privateEnterpriseNumber */
            ti = proto_tree_add_item(pdutree, hf_cflow_private_enterprise_number,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 34000: /* cts_sgt_source_tag */
            ti = proto_tree_add_item(pdutree, hf_cflow_cts_sgt_source_tag,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 34001: /* cts_sgt_destination_tag */
            ti = proto_tree_add_item(pdutree, hf_cflow_cts_sgt_destination_tag,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 34002: /* cts_sgt_source_name */
            ti = proto_tree_add_item(pdutree, hf_cflow_cts_sgt_source_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 34003: /* cts_sgt_destination_name */
            ti = proto_tree_add_item(pdutree, hf_cflow_cts_sgt_destination_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case 37000: /* packets_dropped */
            ti = proto_tree_add_item(pdutree, hf_cflow_packets_dropped,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37003: /* byte_rate */
            ti = proto_tree_add_item(pdutree, hf_cflow_byte_rate,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37004: /* application_media_bytes */
            ti = proto_tree_add_item(pdutree, hf_cflow_application_media_bytes,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37006: /* application_media_byte_rate */
            ti = proto_tree_add_item(pdutree, hf_cflow_application_media_byte_rate,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37007: /* application_media_packets */
            ti = proto_tree_add_item(pdutree, hf_cflow_application_media_packets,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37009: /* application_media_packet_rate */
            ti = proto_tree_add_item(pdutree, hf_cflow_application_media_packet_rate,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37011: /* application_media_event */
            ti = proto_tree_add_item(pdutree, hf_cflow_application_media_event,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 37012: /* monitor_event */
            ti = proto_tree_add_item(pdutree, hf_cflow_monitor_event,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case 37013: /* timestamp_interval */
            ti = proto_tree_add_item(pdutree, hf_cflow_timestamp_interval,
                                     tvb, offset, length, ENC_TIME_TIMESPEC|ENC_BIG_ENDIAN);
            break;
        case 37014: /* transport_packets_expected */
            ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_expected,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37016: /* transport_round_trip_time */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_round_trip_time_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                /* value is in microseconds, adjust to nanoseconds*/
                ts.secs =0;
                ts.nsecs= tvb_get_ntohl(tvb,offset) * 1000;
                ti = proto_tree_add_time(pdutree, hf_cflow_transport_round_trip_time,
                                         tvb, offset, length, &ts);
            }
            break;
        case 37017: /* transport_event_packet_loss */
            ti = proto_tree_add_item(pdutree, hf_cflow_transport_event_packet_loss,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37019: /* transport_packets_lost */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_lost_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_lost,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37021: /* transport_packets_lost_rate */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_lost_rate_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_lost_rate,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37022: /* transport_rtp_ssrc */
            ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_ssrc,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 37023: /* transport_rtp_jitter_mean */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_jitter_mean_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                /* value is in microseconds, adjust to nanoseconds*/
                ts.secs =0;
                ts.nsecs= tvb_get_ntohl(tvb,offset) * 1000;

                ti = proto_tree_add_time(pdutree, hf_cflow_transport_rtp_jitter_mean,
                                         tvb, offset, length, &ts);
            }
            break;
        case 37024: /* transport_rtp_jitter_min */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_jitter_min_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                /* value is in microseconds, adjust to nanoseconds*/
                ts.secs =0;
                ts.nsecs= tvb_get_ntohl(tvb,offset) * 1000;
                ti = proto_tree_add_time(pdutree, hf_cflow_transport_rtp_jitter_min,
                                         tvb, offset, length, &ts);
            }
            break;
        case 37025: /* transport_rtp_jitter_max */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF ) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_jitter_max_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                /* value is in microseconds, adjust to nanoseconds*/
                ts.secs =0;
                ts.nsecs= tvb_get_ntohl(tvb,offset) * 1000;
                ti = proto_tree_add_time(pdutree, hf_cflow_transport_rtp_jitter_max,
                                         tvb, offset, length, &ts);
            }
            break;
        case 37041: /* transport_payload_type */
            if (tvb_get_guint8(tvb,offset)== 0xFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_payload_type_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_rtp_payload_type,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37071: /* bytes_out_of_order */
            if (tvb_get_ntoh64(tvb,offset)== G_GINT64_CONSTANT(0xFFFFFFFFFFFFFFFF)) {
                /* need to add custom code to show "Not Measured"  */
                proto_tree_add_text(pdutree, tvb, offset, 8,
                                    "Transport Bytes Out of Order: Not Measured (0x%"G_GINT64_MODIFIER"x)",
                                    tvb_get_ntoh64(tvb,offset));
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_bytes_out_of_order,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_bytes_out_of_order,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37074: /* packets_out_of_order */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_packets_out_of_order_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_bytes_out_of_order,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37083: /* tcp_window_size_min */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_min_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_min,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;

        case 37084: /* tcp_window_size_max */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_max_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_max,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;

        case 37085: /* tcp_window_size_mean */
            if (tvb_get_ntohl(tvb,offset)== 0xFFFFFFFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_mean_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_window_size_mean,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
        case 37086: /* tcp_maximum_segment_size */
            if (tvb_get_ntohs(tvb,offset)== 0xFFFF) {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_maximum_segment_size_string,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            } else {
                ti = proto_tree_add_item(pdutree, hf_cflow_transport_tcp_maximum_segment_size,
                                         tvb, offset, length, ENC_BIG_ENDIAN);
            }
            break;
            /* Ericsson SE NAT Logging */
        case 24628: /* natContextId */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_context_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 24629: /* natContextName */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_context_name,
                                     tvb, offset, length, ENC_UTF_8|ENC_NA);
            break;
        case 24630: /* natAssignTime */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_nat_assign_time,
                                     tvb, offset, length, &ts);
            break;
        case 24631: /* natUnAssignTime */
            ts.secs = tvb_get_ntohl(tvb, offset);
            ts.nsecs = 0;
            ti = proto_tree_add_time(pdutree, hf_cflow_nat_unassign_time,
                                     tvb, offset, length, &ts);
            break;
        case 24632: /* natInternalAddr */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_int_addr,
                                     tvb, offset, length, ENC_NA);
            break;
        case 24633: /* natExternalAddr */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_ext_addr,
                                     tvb, offset, length, ENC_NA);
            break;
        case 24634: /* natExternalPortFirst */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_ext_port_first,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 24635: /* natExternalPortLast */
            ti = proto_tree_add_item(pdutree, hf_cflow_nat_ext_port_last,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

            /* Cisco ASA 5500 Series */
        case 33000: /* NF_F_INGRESS_ACL_ID */
            proto_tree_add_item(pdutree, hf_cflow_ingress_acl_id,
                                tvb, offset, length, ENC_NA);
            break;
        case 33001: /* NF_F_EGRESS_ACL_ID */
            proto_tree_add_item(pdutree, hf_cflow_egress_acl_id,
                                tvb, offset, length, ENC_NA);
            break;
        case 33002: /* NF_F_FW_EXT_EVENT */
            proto_tree_add_item(pdutree, hf_cflow_fw_ext_event,
                                tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case 40000: /* NF_F_USERNAME[_MAX] */
            proto_tree_add_item(pdutree, hf_cflow_aaa_username,
                                tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

            /* CACE Technologies */
        case VENDOR_CACE << 16 | 0: /* caceLocalIPv4Address */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_ipv4_address,
                                     tvb, offset, length, ENC_NA);
            TVB_SET_ADDRESS(&local_addr, AT_IPv4, tvb, offset, 4);
            got_flags |= GOT_LOCAL_ADDR;
            break;

        case VENDOR_CACE << 16 | 1: /* caceRemoteIPv4Address */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_remote_ipv4_address,
                                     tvb, offset, length, ENC_NA);
            TVB_SET_ADDRESS(&remote_addr, AT_IPv4, tvb, offset, 4);
            got_flags |= GOT_REMOTE_ADDR;
            break;

        case VENDOR_CACE << 16 | 2: /* caceLocalIPv6Address */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_ipv6_address,
                                     tvb, offset, length, ENC_NA);
            TVB_SET_ADDRESS(&local_addr, AT_IPv6, tvb, offset, 16);
            got_flags |= GOT_LOCAL_ADDR;
            break;

        case VENDOR_CACE << 16 | 3: /* caceRemoteIPv6Address */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_remote_ipv6_address,
                                     tvb, offset, length, ENC_NA);
            TVB_SET_ADDRESS(&remote_addr, AT_IPv6, tvb, offset, 16);
            got_flags |= GOT_REMOTE_ADDR;
            break;

        case VENDOR_CACE << 16 | 4: /* caceLocalTransportPort */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            local_port = tvb_get_ntohs(tvb, offset);
            got_flags |= GOT_LOCAL_PORT;
            break;

        case VENDOR_CACE << 16 | 5: /* caceRemoteTransportPort */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_remote_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            remote_port = tvb_get_ntohs(tvb, offset);
            got_flags |= GOT_REMOTE_PORT;
            break;

        case VENDOR_CACE << 16 | 6: /* caceLocalIPv4id */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_ipv4_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            /*ipv4_id = tvb_get_ntohs(tvb, offset);*/
            /*got_flags |= GOT_IPv4_ID;*/
            break;

        case VENDOR_CACE << 16 | 7: /* caceLocalICMPid */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_icmp_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            /*icmp_id = tvb_get_ntohs(tvb, offset);*/
            /*got_flags |= GOT_ICMP_ID;*/
            break;

        case VENDOR_CACE << 16 | 8: /* caceLocalProcessUserId */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_uid,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            uid = tvb_get_ntohl(tvb, offset);
            got_flags |= GOT_UID;
            break;

        case VENDOR_CACE << 16 | 9: /* caceLocalProcessId */
            ti = proto_tree_add_item(pdutree, hf_pie_cace_local_pid,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            pid = tvb_get_ntohl(tvb, offset);
            got_flags |= GOT_PID;
            break;

        case VENDOR_CACE << 16 | 10: /* caceLocalProcessUserName */
            uname_len = tvb_get_guint8(tvb, offset);
            uname_str = tvb_format_text(tvb, offset+1, uname_len);
            proto_tree_add_item(pdutree, hf_pie_cace_local_username_len,
                                tvb, offset, 1, ENC_NA);
            ti = proto_tree_add_string(pdutree, hf_pie_cace_local_username,
                                       tvb, offset+1, uname_len, uname_str);
            length = uname_len + 1;
            got_flags |= GOT_USERNAME;
            break;

        case VENDOR_CACE << 16 | 11: /* caceLocalProcessCommand */
            cmd_len = tvb_get_guint8(tvb, offset);
            cmd_str = tvb_format_text(tvb, offset+1, cmd_len);
            proto_tree_add_item(pdutree, hf_pie_cace_local_cmd_len,
                                tvb, offset, 1, ENC_NA);
            ti = proto_tree_add_string(pdutree, hf_pie_cace_local_cmd,
                                       tvb, offset+1, cmd_len, cmd_str);
            length = cmd_len + 1;
            got_flags |= GOT_COMMAND;
            break;

            /* START NTOP */
        case (NTOP_BASE + 80):           /* FRAGMENTED */
        case ((VENDOR_NTOP << 16) | 80): /* FRAGMENTED */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_fragmented,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 81):           /* FINGERPRINT */
        case ((VENDOR_NTOP << 16) | 81): /* FINGERPRINT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_fingerprint,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 82):           /* CLIENT_NW_DELAY_SEC */
        case ((VENDOR_NTOP << 16) | 82): /* CLIENT_NW_DELAY_SEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_client_nw_delay_sec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 83): /*           /\* CLIENT_NW_DELAY_USEC *\/ */
        case ((VENDOR_NTOP << 16) | 83): /* CLIENT_NW_DELAY_USEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_client_nw_delay_usec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 84):           /* SERVER_NW_DELAY_SEC */
        case ((VENDOR_NTOP << 16) | 84): /* SERVER_NW_DELAY_SEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_server_nw_delay_sec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 85):           /* SERVER_NW_DELAY_USEC */
        case ((VENDOR_NTOP << 16) | 85): /* SERVER_NW_DELAY_USEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_server_nw_delay_usec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 86):           /* APPL_LATENCY_SEC */
        case ((VENDOR_NTOP << 16) | 86): /* APPL_LATENCY_SEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_appl_latency_sec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 87):           /* APPL_LATENCY_USEC */
        case ((VENDOR_NTOP << 16) | 87): /* APPL_LATENCY_USEC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_appl_latency_sec,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 98):           /* ICMP_FLAGS */
        case ((VENDOR_NTOP << 16) | 98): /* ICMP_FLAGS */
            /* Cumulative of all flow ICMP types */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_icmp_flags,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 101):           /* SRC_IP_COUNTRY */
        case ((VENDOR_NTOP << 16) | 101): /* SRC_IP_COUNTRY */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_src_ip_country,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;
        case (NTOP_BASE + 102):           /* SRC_IP_CITY */
        case ((VENDOR_NTOP << 16) | 102): /* SRC_IP_CITY */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_src_ip_city,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;
        case (NTOP_BASE + 103):           /* DST_IP_COUNTRY */
        case ((VENDOR_NTOP << 16) | 103): /* DST_IP_COUNTRY */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_dst_ip_country,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;
        case (NTOP_BASE + 104):           /* DST_IP_CITY */
        case ((VENDOR_NTOP << 16) | 104): /* DST_IP_CITY */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_dst_ip_city,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;
        case (NTOP_BASE + 105):           /* FLOW_PROTO_PORT */
        case ((VENDOR_NTOP << 16) | 105): /* FLOW_PROTO_PORT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_flow_proto_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case (NTOP_BASE + 106):           /* TUNNEL_ID */
        case ((VENDOR_NTOP << 16) | 106): /* TUNNEL_ID */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_tunnel_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 107):           /* LONGEST_FLOW_PKT */
        case ((VENDOR_NTOP << 16) | 107): /* LONGEST_FLOW_PKT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_longest_flow_pkt,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 108):           /* SHORTEST_FLOW_PKT */
        case ((VENDOR_NTOP << 16) | 108): /* SHORTEST_FLOW_PKT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_shortest_flow_pkt,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 109):           /* RETRANSMITTED_IN_PKTS */
        case ((VENDOR_NTOP << 16) | 109): /* RETRANSMITTED_IN_PKTS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_retransmitted_in_pkts,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 110):           /* RETRANSMITTED_OUT_PKTS */
        case ((VENDOR_NTOP << 16) | 110): /* RETRANSMITTED_OUT_PKTS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_retransmitted_out_pkts,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 111):           /* OOORDER_IN_PKTS */
        case ((VENDOR_NTOP << 16) | 111): /* OOORDER_IN_PKTS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_ooorder_in_pkts,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 112):           /* OOORDER_OUT_PKTS */
        case ((VENDOR_NTOP << 16) | 112): /* OOORDER_OUT_PKTS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_ooorder_out_pkts,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 113):           /* UNTUNNELED_PROTOCOL */
        case ((VENDOR_NTOP << 16) | 113): /* UNTUNNELED_PROTOCOL */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_untunneled_protocol,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 114):           /* UNTUNNELED_IPV4_SRC_ADDR */
        case ((VENDOR_NTOP << 16) | 114): /* UNTUNNELED_IPV4_SRC_ADDR */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_untunneled_ipv4_src_addr,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 115):           /* UNTUNNELED_L4_SRC_PORT */
        case ((VENDOR_NTOP << 16) | 115): /* UNTUNNELED_L4_SRC_PORT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_untunneled_l4_src_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 116):           /* UNTUNNELED_IPV4_DST_ADDR */
        case ((VENDOR_NTOP << 16) | 116): /* UNTUNNELED_IPV4_DST_ADDR */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_untunneled_ipv4_dst_addr,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 117):           /* UNTUNNELED_L4_DST_PORT */
        case ((VENDOR_NTOP << 16) | 117): /* UNTUNNELED_L4_DST_PORT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_untunneled_l4_dst_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case (NTOP_BASE + 120):           /* DUMP_PATH */
        case ((VENDOR_NTOP << 16) | 120): /* DUMP_PATH */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_dump_path,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;

        case (NTOP_BASE + 130):           /* SIP_CALL_ID */
        case ((VENDOR_NTOP << 16) | 130): /* SIP_CALL_ID */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_sip_call_id,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 131):           /* SIP_CALLING_PARTY */
        case ((VENDOR_NTOP << 16) | 131): /* SIP_CALLING_PARTY */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_sip_calling_party,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 132):           /* SIP_CALLED_PARTY */
        case ((VENDOR_NTOP << 16) | 132): /* SIP_CALLED_PARTY */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_sip_called_party,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 133):           /* SIP_RTP_CODECS */
        case ((VENDOR_NTOP << 16) | 133): /* SIP_RTP_CODECS */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_sip_rtp_codecs,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 134):           /* SIP_INVITE_TIME */
        case ((VENDOR_NTOP << 16) | 134): /* SIP_INVITE_TIME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_invite_time,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 135):           /* SIP_TRYING_TIME */
        case ((VENDOR_NTOP << 16) | 135): /* SIP_TRYING_TIME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_trying_time,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 136):           /* SIP_RINGING_TIME */
        case ((VENDOR_NTOP << 16) | 136): /* SIP_RINGING_TIME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_ringing_time,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 137):           /* SIP_OK_TIME */
        case ((VENDOR_NTOP << 16) | 137): /* SIP_OK_TIME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_ok_time,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 138):           /* SIP_BYE_TIME */
        case ((VENDOR_NTOP << 16) | 138): /* SIP_BYE_TIME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_bye_time,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 139):           /* SIP_RTP_SRC_IP */
        case ((VENDOR_NTOP << 16) | 139): /* SIP_RTP_SRC_IP */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_rtp_src_ip,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 140):           /* SIP_RTP_SRC_PORT */
        case ((VENDOR_NTOP << 16) | 140): /* SIP_RTP_SRC_PORT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_rtp_src_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 141):           /* SIP_RTP_DST_IP */
        case ((VENDOR_NTOP << 16) | 141): /* SIP_RTP_DST_IP */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_rtp_dst_ip,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 142):           /* SIP_RTP_DST_PORT */
        case ((VENDOR_NTOP << 16) | 142): /* SIP_RTP_DST_PORT */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_sip_rtp_dst_port,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 150):           /* RTP_FIRST_SSRC */
        case ((VENDOR_NTOP << 16) | 150): /* RTP_FIRST_SSRC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_first_ssrc,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 151):           /* RTP_FIRST_TS */
        case ((VENDOR_NTOP << 16) | 151): /* RTP_FIRST_TS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_first_ts,
                                     tvb, offset, length, ENC_TIME_TIMESPEC|ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 152):           /* RTP_LAST_SSRC */
        case ((VENDOR_NTOP << 16) | 152): /* RTP_LAST_SSRC */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_last_ssrc,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 153):           /* RTP_LAST_TS */
        case ((VENDOR_NTOP << 16) | 153): /* RTP_LAST_TS */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_last_ts,
                                     tvb, offset, length, ENC_TIME_TIMESPEC|ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 154):           /* RTP_IN_JITTER */
        case ((VENDOR_NTOP << 16) | 154): /* RTP_IN_JITTER */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_in_jitter,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 155):           /* RTP_OUT_JITTER */
        case ((VENDOR_NTOP << 16) | 155): /* RTP_OUT_JITTER */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_out_jitter,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 156):           /* RTP_IN_PKT_LOST */
        case ((VENDOR_NTOP << 16) | 156): /* RTP_IN_PKT_LOST */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_in_pkt_lost,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 157):           /* RTP_OUT_PKT_LOST */
        case ((VENDOR_NTOP << 16) | 157): /* RTP_OUT_PKT_LOST */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_out_pkt_lost,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 158):           /* RTP_OUT_PAYLOAD_TYPE */
        case ((VENDOR_NTOP << 16) | 158): /* RTP_OUT_PAYLOAD_TYPE */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_out_payload_type,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 159):           /* RTP_IN_MAX_DELTA */
        case ((VENDOR_NTOP << 16) | 159): /* RTP_IN_MAX_DELTA */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_in_max_delta,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 160):           /* RTP_OUT_MAX_DELTA */
        case ((VENDOR_NTOP << 16) | 160): /* RTP_OUT_MAX_DELTA */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_rtp_out_max_delta,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 168):           /* PROC_ID */
        case ((VENDOR_NTOP << 16) | 168): /* PROC_ID */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_proc_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case (NTOP_BASE + 169):           /* PROC_NAME */
        case ((VENDOR_NTOP << 16) | 169): /* PROC_NAME */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_proc_name,
                                     tvb, offset, length, ENC_ASCII|ENC_NA);
            break;
        case (NTOP_BASE + 180):           /* HTTP_URL */
        case ((VENDOR_NTOP << 16) | 180): /* HTTP_URL */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_http_url,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 181):           /* HTTP_RET_CODE */
        case ((VENDOR_NTOP << 16) | 181): /* HTTP_RET_CODE */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_http_ret_code,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;


        case (NTOP_BASE + 182):           /* HTTP_REFERER */
        case ((VENDOR_NTOP << 16) | 182): /* HTTP_REFERER */
            break;
        case (NTOP_BASE + 183):           /* HTTP_UA */
        case ((VENDOR_NTOP << 16) | 183): /* HTTP_UA */
            break;
        case (NTOP_BASE + 184):           /* HTTP_MIME */
        case ((VENDOR_NTOP << 16) | 184): /* HTTP_MIME */
            break;

        case (NTOP_BASE + 185):           /* SMTP_MAIL_FROM */
        case ((VENDOR_NTOP << 16) | 185): /* SMTP_MAIL_FROM */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_smtp_mail_from,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 186):           /* SMTP_RCPT_TO */
        case ((VENDOR_NTOP << 16) | 186): /* SMTP_RCPT_TO */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_smtp_rcpt_to,
                                       tvb, offset, length, gen_str);
            break;

        case (NTOP_BASE + 190):           /* FLOW_ID */
        case ((VENDOR_NTOP << 16) | 190): /* FLOW_ID */
            ti = proto_tree_add_item(pdutree, hf_cflow_flow_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;

        case (NTOP_BASE + 195):           /* MYSQL_SERVER_VERSION */
        case ((VENDOR_NTOP << 16) | 195): /* MYSQL_SERVER_VERSION */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_mysql_server_version,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 196):           /* MYSQL_USERNAME */
        case ((VENDOR_NTOP << 16) | 196): /* MYSQL_USERNAME */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_mysql_username,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 197):           /* MYSQL_DB */
        case ((VENDOR_NTOP << 16) | 197): /* MYSQL_DB */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_mysql_db,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 198):           /* MYSQL_QUERY */
        case ((VENDOR_NTOP << 16) | 198): /* MYSQL_QUERY */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_ntop_mysql_query,
                                       tvb, offset, length, gen_str);
            break;
        case (NTOP_BASE + 199):           /* MYSQL_RESPONSE */
        case ((VENDOR_NTOP << 16) | 199): /* MYSQL_RESPONSE */
            ti = proto_tree_add_item(pdutree, hf_pie_ntop_mysql_response,
                                     tvb, offset, length, ENC_BIG_ENDIAN);

            break;

            /* END NTOP */

            /* START Plixer International */
        case ((VENDOR_PLIXER << 16) | 100):    /* client_ip_v4 */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_client_ip_v4,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 101):    /* client_hostname */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_client_hostname,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 102):    /* partner_name */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_partner_name,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 103):    /* server_hostname */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_server_hostname,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 104):    /* server_ip_v4 */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_server_ip_v4,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 105):    /* recipient_address */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_recipient_address,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 106):    /* event_id */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_event_id,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 107):    /* msgid */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_msgid,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 108):    /* priority */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_priority,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 109):    /* recipient_report_status */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_recipient_report_status,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 110):    /* number_recipients */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_number_recipients,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 111):    /* origination_time */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_origination_time,
                                     tvb, offset, length, ENC_TIME_TIMESPEC|ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 112):    /* encryption */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_encryption,
                                     tvb, offset, length, ENC_BIG_ENDIAN);
            break;
        case ((VENDOR_PLIXER << 16) | 113):    /* service_version */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_service_version,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 114):    /* linked_msgid */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_linked_msgid,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 115):    /* message_subject */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_message_subject,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 116):    /* sender_address */
            gen_str = tvb_format_text(tvb, offset, length);
            ti = proto_tree_add_string(pdutree, hf_pie_plixer_sender_address,
                                       tvb, offset, length, gen_str);
            break;
        case ((VENDOR_PLIXER << 16) | 117):    /* date_time */
            ti = proto_tree_add_item(pdutree, hf_pie_plixer_date_time,
                                     tvb, offset, length, ENC_TIME_TIMESPEC|ENC_BIG_ENDIAN);
            break;
            /* END Plixer International */

        default:  /* Unknown Field ID */
            if ((hdrinfo_p->vspec == 9) || (pen == REVPEN)) {
                ti = proto_tree_add_bytes_format_value(pdutree, hf_cflow_unknown_field_type,
                                                       tvb, offset, length, NULL,
                                                       "Type %u: Value (hex bytes): %s",
                                                       masked_type,
                                                       tvb_bytes_to_ep_str_punct(tvb, offset, length, ' '));
            } else { /* v10 PEN */
                ti = proto_tree_add_bytes_format_value(pdutree, hf_ipfix_enterprise_private_entry,
                                                       tvb, offset, length, NULL,
                                                       "(%s) Type %u: Value (hex bytes): %s",
                                                       pen_str ? pen_str : "(null)",
                                                       masked_type,
                                                       tvb_bytes_to_ep_str_punct(tvb, offset, length, ' '));
            }
            break;

        } /* switch (pen_type) */

        if (ti && (vstr_len != 0)) {
            /* XXX: ugh: not very pretty: how to show/highlight actual length bytes ?? */
            /* YYY: added the length in a tree.  Not sure if this is best.  */
            proto_item_append_text(ti, " (Variable Length)");
            PROTO_ITEM_SET_GENERATED(ti);
            string_tree = proto_item_add_subtree(ti, ett_str_len);
            proto_tree_add_uint(string_tree, hf_string_len_short, tvb,
                                gen_str_offset-vstr_len, 1, string_len_short);
            if (vstr_len == 3) {
                proto_tree_add_uint(string_tree, hf_string_len_long, tvb,
                                    gen_str_offset-2, 2, string_len_long);
            }



        }

        if (ti && (pen == REVPEN)) {
            /* XXX: why showing type ? type not shown if not reverse */
            proto_item_append_text(ti, " (Reverse Type %u %s)",
                                   masked_type,
                                   val_to_str_ext_const(masked_type, &v9_v10_template_types_ext,"Unknown"));
        }

        offset += length;
    } /* for (i=0; i < count; i++) */

    /* If only "start" or "end" time, show it here */
    /* XXX: length is actually 8 if millisec, microsec, nanosec time */
    for (i = 0; i < 2; i++) {
        if (!(offset_s[i] && offset_e[i])) {
            if (offset_s[i]) {
                if (msec_start[i]) {
                    proto_tree_add_time(pdutree, hf_cflow_timestart, tvb,
                                        offset_s[i], 4, &ts_start[i]);
                } else {
                    proto_tree_add_time(pdutree, hf_cflow_abstimestart, tvb,
                                        offset_s[i], 4, &ts_start[i]);
                }
            }
            if (offset_e[i]) {
                if (msec_end[i]) {
                    proto_tree_add_time(pdutree, hf_cflow_timeend, tvb,
                                        offset_e[i], 4, &ts_end[i]);
                } else {
                    proto_tree_add_time(pdutree, hf_cflow_abstimeend, tvb,
                                        offset_e[i], 4, &ts_end[i]);
                }
            }
        }
    }

    /* XXX - These IDs are currently hard-coded in procflow.py. */
    if (got_flags == GOT_TCP_UDP && (tmplt_p->tmplt_id == 256 || tmplt_p->tmplt_id == 258)) {
        add_tcp_process_info(pinfo->fd->num, &local_addr, &remote_addr, local_port, remote_port, uid, pid, uname_str, cmd_str);
    }
    if (got_flags == GOT_TCP_UDP && (tmplt_p->tmplt_id == 257 || tmplt_p->tmplt_id == 259)) {
        add_udp_process_info(pinfo->fd->num, &local_addr, &remote_addr, local_port, remote_port, uid, pid, uname_str, cmd_str);
    }

    return (guint) (offset - orig_offset);

}
