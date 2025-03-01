static int
CVE_2014_6426_PATCHED_dissect_hip_tlv(tvbuff_t *tvb, int offset, proto_item *ti, int type, int tlv_len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
        proto_tree *t=NULL;
        proto_item *ti_tlv, *ti_loc;
        guint8 n, algorithm, reg_type;
        guint16 trans, hi_len, di_len, di_type, e_len, pv_len;
        guint32 reserved, hi_hdr;
        guint8 transport_proto;
        guint8 locator_type;
        int newoffset, newlen, hi_t;

        /* move over the TLV */
        newoffset = offset + 4;
        switch (type)
        {
        case PARAM_ESP_INFO:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_ei_res, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                /* KEYMAT index */
                newoffset += 2;
                proto_tree_add_item(t, hf_hip_tlv_ei_keyidx, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                /* OLD SPI */
                newoffset += 2;
                proto_tree_add_item(t, hf_hip_tlv_ei_oldspi, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                /* NEW SPI */
                newoffset += 4;
                proto_tree_add_item(t, hf_hip_tlv_ei_newspi, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                break;
        case PARAM_R1_COUNTER:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_r1_res, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                /* R1 generation counter */
                newoffset += 4;
                proto_tree_add_item(t, hf_hip_tlv_r1count, tvb, newoffset, 8, ENC_NA);
                break;
        case PARAM_LOCATOR:
                /* RFC 5206 section 4. and  RFC 5770 section 5.7. for type 2 locators
                 */
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                tlv_len -= 4;
                /* loop through included locators */
                while (tlv_len > 0) {
                        /* Every locator to new tree node
                         * Skip ahead and read the 0 or 1 type locator from 8 bytes
                         * and type 2 locator from 20 bytes to be used as the top level
                         * tree_item for this subtree
                         */
                        locator_type = tvb_get_guint8(tvb, newoffset + 1);
                        if (locator_type == 0) {
                                ti_loc = proto_tree_add_item(t, hf_hip_tlv_locator_address,
                                                             tvb, newoffset + 8, 16, ENC_NA);
                        } else if (locator_type == 1) {
                                ti_loc = proto_tree_add_item(t, hf_hip_tlv_locator_address,
                                                             tvb, newoffset + 12, 16, ENC_NA);
                        } else if (locator_type == 2) {
                                ti_loc = proto_tree_add_item(t, hf_hip_tlv_locator_address,
                                                             tvb, newoffset + 20, 16, ENC_NA);
                        } else {
                                /* unknown or malformed locator type jumping over it */
                                ti_loc = NULL;
                                newoffset += (1 + tvb_get_guint8(tvb, newoffset + 2));
                                tlv_len -= (1 + tvb_get_guint8(tvb, newoffset + 2));
                        }
                        if (locator_type <= 2) {
                                ti_loc = proto_item_add_subtree(ti_loc, ett_hip_locator_data);
                                /* Traffic type */
                                proto_tree_add_item(ti_loc, hf_hip_tlv_locator_traffic_type, tvb,
                                                    newoffset, 1, ENC_BIG_ENDIAN);
                                newoffset++;
                                /* Locator type */
#if 0
                                locator_type = tvb_get_guint8(tvb, newoffset);
#endif
                                proto_tree_add_item(ti_loc, hf_hip_tlv_locator_type, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                                newoffset++;
                                /* Locator length */
                                proto_tree_add_item(ti_loc, hf_hip_tlv_locator_len, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                                newoffset++;
                                /* Reserved includes the Preferred bit */
                                reserved = tvb_get_guint8(tvb, newoffset);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                                proto_tree_add_uint_format_value(ti_loc, hf_hip_tlv_locator_reserved, tvb,
                                                           newoffset, 1, reserved,
                                                           "0x%x %s", reserved,
                                                           (reserved >> 31) ? "(Preferred)" : "");
                                newoffset++;
                                /* Locator lifetime */
                                proto_tree_add_item(ti_loc, hf_hip_tlv_locator_lifetime, tvb,
                                                    newoffset, 4, ENC_BIG_ENDIAN);
                                newoffset += 4;
                                if (locator_type == 0) {
                                        /* Locator types 1 and 0 RFC 5206 section 4.2.*/
                                        /* Locator */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_address,
                                                            tvb, newoffset, 16, ENC_NA);
                                        newoffset += 16;
                                        tlv_len -= 24;
                                } else if (locator_type == 1) {
                                        /* Locator types 1 and 0 RFC 5206 section 4.2.*/
                                        /* SPI */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_spi, tvb,
                                                            newoffset, 4, ENC_BIG_ENDIAN);
                                        newoffset += 4;
                                        /* Locator */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_address,
                                                            tvb, newoffset, 16, ENC_NA);
                                        newoffset += 16;
                                        tlv_len -= 28;
                                } else if (locator_type == 2) {
                                        /* Locator type 2 RFC 5770 section 5.7. */
                                        /* Tansport port */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_port, tvb,
                                                            newoffset, 2, ENC_BIG_ENDIAN);
                                        newoffset += 2;
                                        /* Transport protocol */
                                        transport_proto = tvb_get_guint8(tvb, newoffset);
                                        /* RFC 5770 section 5.6 */
                                        proto_tree_add_uint_format(ti_loc, hf_hip_tlv_locator_transport_protocol,
                                                                   tvb, newoffset, 1, transport_proto,
                                                                   "Transport protocol: %d %s",
                                                                   transport_proto,
                                                                   (transport_proto == 17) ?
                                                                   "(UDP)" : "");
                                        newoffset++;
                                        /* Kind */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_kind, tvb,
                                                            newoffset, 1, ENC_BIG_ENDIAN);
                                        newoffset++;
                                        /* Priority */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_priority, tvb,
                                                            newoffset, 4, ENC_BIG_ENDIAN);
                                        newoffset += 4;
                                        /* SPI */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_spi, tvb,
                                                            newoffset, 4, ENC_BIG_ENDIAN);
                                        newoffset += 4;
                                        /* Locator */
                                        proto_tree_add_item(ti_loc, hf_hip_tlv_locator_address,
                                                            tvb, newoffset, 16, ENC_NA);
                                        newoffset += 16;
                                        tlv_len -= 36;
                                }
                        }
                }
                break;
        case PARAM_PUZZLE:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* K number of verified bits */
                proto_tree_add_item(t, hf_hip_tlv_puzzle_k, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                /* Puzzle lifetime */
                newoffset++;
                proto_tree_add_item(t, hf_hip_tlv_puzzle_life, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                /* Puzzle O*/
                newoffset++;
                proto_tree_add_item(t, hf_hip_tlv_puzzle_o, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                /* Puzzle I */
                newoffset += 2;
                proto_tree_add_item(t, hf_hip_tlv_puzzle_i, tvb,newoffset, tlv_len - 4, ENC_NA);
                break;
        case PARAM_SOLUTION:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* K number of verified bits */
                proto_tree_add_item(t, hf_hip_tlv_solution_k, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                /* Solution Reserved */
                newoffset++;
                proto_tree_add_item(t, hf_hip_tlv_solution_reserved, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                /* Solution Opaque */
                newoffset++;
                proto_tree_add_item(t, hf_hip_tlv_solution_o, tvb,newoffset, 2, ENC_BIG_ENDIAN);
                /* Solution I */
                newoffset += 2;
                proto_tree_add_item(t, hf_hip_tlv_solution_i, tvb, newoffset, (tlv_len - 4)/2, ENC_NA);
                /* Solution J */
                newoffset += (tlv_len - 4) /2;
                proto_tree_add_item(t, hf_hip_tlv_solution_j, tvb, newoffset, (tlv_len -4)/2, ENC_NA);
                break;
        case PARAM_SEQ:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Update ID */
                proto_tree_add_item(t, hf_hip_tlv_seq_updid, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                break;
        case PARAM_ACK:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Can contain multiple Update IDs from peer */
                while (tlv_len > 0) {
                        /* peer Update ID */
                        proto_tree_add_item(t, hf_hip_tlv_ack_updid, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                        newoffset += 4;
                        tlv_len -= 4;
                }
                break;
        case PARAM_DIFFIE_HELLMAN:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                n = tvb_get_guint8(tvb, newoffset);
                /* First Group ID*/
                proto_tree_add_uint_format(t, hf_hip_tlv_dh_group_id, tvb, newoffset,
                                           1, n, "%u (%s)", n,
                                           val_to_str_const(n, dh_group_id_vals, "Unknown"));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                /* First Public value len */
                newoffset++;
                pv_len = tvb_get_ntohs(tvb, newoffset);
                proto_tree_add_item(t, hf_hip_tlv_dh_pv_length, tvb, newoffset, 2, ENC_BIG_ENDIAN);

                /* First Public value */
                newoffset += 2;
                proto_tree_add_item(t, hf_hip_tlv_dh_pub, tvb, newoffset, pv_len, ENC_NA);
                /* Check for the second group */
                if ((pv_len + newoffset) < tlv_len) {
                        /* Second Group ID*/
                        newoffset += pv_len;
                        proto_tree_add_uint_format(t, hf_hip_tlv_dh_group_id, tvb, newoffset,
                                                   1, n, "%u (%s)", n,
                                                   val_to_str_const(n, dh_group_id_vals, "Unknown"));
                        /* Second Public value len */
                        newoffset += 1;
                        pv_len = tvb_get_ntohs(tvb, newoffset);
                        proto_tree_add_item(t, hf_hip_tlv_dh_pv_length, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                        /* Second Public Value */
                        newoffset += 2;
                        proto_tree_add_item(t, hf_hip_tlv_dh_pub, tvb, newoffset,
                                             pv_len, ENC_NA);
                }
                break;
        case PARAM_ESP_TRANSFORM:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_esp_reserved, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset +=2;
                tlv_len -= 2;
                while (tlv_len > 0) {
                        /* Suite # 1, 2, ...,  n
                         * two bytes per transform id
                         */
                        trans = tvb_get_ntohs(tvb, newoffset);
                        proto_tree_add_uint_format(t, hf_hip_tlv_trans_id, tvb,
                                                   newoffset, 2, trans, "%u (%s)", trans,
                                                   val_to_str_const(trans, transform_id_vals, "Unknown"));
                        tlv_len -= 2;
                        newoffset += 2;
                }
                break;
        case PARAM_HIP_TRANSFORM:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                while (tlv_len > 0) {
                        /* Suite # 1, 2, ...,  n
                           two bytes per transform id */
                        trans = tvb_get_ntohs(tvb, newoffset);
                        proto_tree_add_uint_format(t, hf_hip_tlv_trans_id, tvb,
                                                   newoffset, 2, trans, "%u (%s)", trans,
                                                   val_to_str_const(trans, transform_id_vals, "Unknown"));
                        tlv_len -= 2;
                        newoffset += 2;
                }
                break;
        case PARAM_NAT_TRAVERSAL_MODE:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_esp_reserved, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                tlv_len -= 2;
                while (tlv_len > 0) {
                        /* Suite # 1, 2, ...,  n
                           two bytes per mode id */
                        trans = tvb_get_ntohs(tvb, newoffset);
                        proto_tree_add_uint_format(t, hf_hip_tlv_nat_traversal_mode_id, tvb,
                                                   newoffset, 2, trans, "%u (%s)", trans,
                                                   val_to_str_const(trans, mode_id_vals, "Unknown"));
                        tlv_len -= 2;
                        newoffset += 2;
                }
                break;
        case PARAM_TRANSACTION_PACING:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Min Ta */
                proto_tree_add_item(t, hf_hip_tlv_transaction_minta, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                break;
        case PARAM_ENCRYPTED:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_enc_reserved, tvb, newoffset, 4, ENC_BIG_ENDIAN);
                newoffset += 4;
                /* IV
                 * 16 bytes IV for AES CBC RFC 3602
                 *  8 bytes IV for 3DES CBC RFC 2405
                 *  0 bytes IV for NULL
                 *  and
                 *  encrypted data after that.
                 */
                proto_tree_add_text(t, tvb, newoffset, tlv_len - 4,
                                    "Encrypted Parameter Data (%u bytes)",  tlv_len - 4);
                break;
        case PARAM_HIP_CIPHER:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                while (tlv_len > 0) {
                        /* Suite # 1, 2, ...,  n
                           two bytes per Cipher Suite id */
                        proto_tree_add_item(t, hf_hip_tlv_cipher_id, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                        tlv_len -= 2;
                        newoffset += 2;
                }
                break;
        case PARAM_HIT_SUITE_LIST:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                while (tlv_len > 0) {
                        /* Suite # 1, 2, ...,  n
                           one byte per HIT Suite id.
                           According to specification, HIT_SUITE_LIST is defined as eight-bit field,
                           current four-bit HIT Suite-IDs only use the four higher order bits in the ID Field.*/
                        proto_tree_add_item(t, hf_hip_tlv_hit_suite_id, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        tlv_len -= 1;
                        newoffset += 1;
                }
                break;
        case PARAM_HOST_ID:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                hi_len = tvb_get_ntohs(tvb, newoffset);
                proto_tree_add_item(t, hf_hip_tlv_host_id_len, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                di_len = tvb_get_ntohs(tvb, newoffset);
                di_type = (di_len >> 12) & 0x000F;        /* get 4 bits for DI type */
                di_len = di_len & 0x0FFF;                /* 12 bits for DI length */
                /* DI type */
                proto_tree_add_item(t, hf_hip_tlv_host_di_type, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                /* DI len */
                proto_tree_add_item(t, hf_hip_tlv_host_di_len, tvb, newoffset, 2, ENC_BIG_ENDIAN);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                newoffset += 2;
                /* hi_hdr - first 4 bytes are 0200ff03 (KEY RR in RFC 2535)
                 *   flags     2  octets
                 *   protocol  1  octet
                 *   algorithm 1  octet (DSA or RSA)
                 *   <public key>
                 */
                hi_hdr = tvb_get_ntohl(tvb, newoffset);
                ti_tlv = proto_tree_add_item(t, hf_hip_tlv_host_id_hdr,
                                             tvb, newoffset, 4, ENC_BIG_ENDIAN);
                if (ti_tlv) {
                        ti_tlv = proto_item_add_subtree(ti_tlv, ett_hip_tlv_host_id_hdr);
                        /* HDR Flags*/
if(judge_para * 5 > 8)  {printf("math exists!"); }
                        proto_tree_add_uint(ti_tlv, hf_hip_tlv_host_id_hdr_flags, tvb,
                                            newoffset, 2, hi_hdr);
                        newoffset += 2;
                        /* HDR Protocol */
                        proto_tree_add_uint(ti_tlv, hf_hip_tlv_host_id_hdr_proto, tvb,
                                            newoffset, 1,  hi_hdr);
                        newoffset += 1;
                        /* HDR Algorithm */
                        proto_tree_add_uint(ti_tlv, hf_hip_tlv_host_id_hdr_alg, tvb,
                                            newoffset, 1, hi_hdr);
                }
                algorithm = tvb_get_guint8(tvb, newoffset);
                switch (algorithm) {
                case HI_ALG_DSA:
                        /* DSA KEY RR RFC 2536
                         *   T         1  octet
                         *   Q         20  octets
                         *   P         64 + T*8  octets
                         *   G         64 + T*8  octets
                         *   Y         64 + T*8  octets
                         */
                        newoffset++; /* 12 + offset */
                        /* T */
                        proto_tree_add_item(t, hf_hip_tlv_host_id_t, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        hi_t = tvb_get_guint8(tvb, newoffset);
                        newoffset++;
                        /* Q */
                        proto_tree_add_item(t, hf_hip_tlv_host_id_q, tvb, newoffset,
                                             20, ENC_NA);
                        newoffset += 20;
                        if (hi_t > 56) /* max 4096 bits */
                                break;
                        /* P */
                        newlen = 64 + (hi_t * 8);
                        proto_tree_add_item(t, hf_hip_tlv_host_id_p, tvb, newoffset,
                                             newlen, ENC_NA);
                        /* G */
                        newoffset += newlen;
                        proto_tree_add_item(t, hf_hip_tlv_host_id_g, tvb, newoffset,
                                             newlen, ENC_NA);
                        /* Y */
                        newoffset += newlen;
                        proto_tree_add_item(t, hf_hip_tlv_host_id_y, tvb, newoffset,
                                             newlen, ENC_NA);
                        break;
                case HI_ALG_RSA:
                        /* RSA KEY RR RFC 3110
                         * e_len        1 or 3 octets
                         * e            specified by e_len
                         * n            variable length public modulus
                         */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                        newoffset++; /* 12 + offset */
                        /* E len */
                        e_len = tvb_get_guint8(tvb, newoffset);
                        proto_tree_add_item(t, hf_hip_tlv_host_id_e_len, tvb, newoffset,
                                            (e_len > 255) ? 3 : 1, ENC_BIG_ENDIAN);
                        newoffset++;
                        hi_len -= 5; /* subtract RDATA + e_len */
                        if (e_len == 0) { /* e_len is 0 followed by 16-bit value */
                                e_len = tvb_get_ntohs(tvb, newoffset);
                                newoffset += 2;
                                hi_len -= 2;
                        }
                        if (e_len > 512) { /* per, RFC 3110 < 4096 bits */
                                proto_tree_add_text(t, tvb, newoffset, 2,
                                                    "<< e_len too large >>");
                                break;
                        }
                        /* e */
                        proto_tree_add_item(t, hf_hip_tlv_host_id_e, tvb, newoffset,
                                             e_len, ENC_NA);
                        newoffset += e_len;
                        hi_len -= e_len;

                        if (hi_len > 512) {
                                proto_tree_add_text(t, tvb, newoffset, 1,
                                                    "<< Invalid HI length >>");
                                break;
                        }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                        /* RSA public modulus n */
                        proto_tree_add_item(t, hf_hip_tlv_host_id_n, tvb, newoffset,
                                             hi_len, ENC_NA);
                        break;
                default:
                        proto_tree_add_text(t, tvb, newoffset, 1,
                                            "Unknown algorithm type (%d).\n", algorithm);

                        break;
                }
                /* FQDN */
                if (di_type == 0)
                        break;
                if (di_type == 1) {
                        /* RFC 1035 */
                        proto_tree_add_text(t, tvb, offset+16+hi_len, di_len,
                                            "FQDN: %s", tvb_get_string (wmem_packet_scope(), tvb, offset+16+hi_len, di_len));
                } else if (di_type == 2) {
                        /* RFC 4282 */
                        proto_tree_add_text(t, tvb, offset+16+hi_len, di_len,
                                            "NAI: %s", tvb_get_string (wmem_packet_scope(), tvb, offset+16+hi_len, di_len));
                }
                break;
        case PARAM_CERT: /* CERT */
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Cert Group */
                proto_tree_add_item(t, hf_hip_tlv_cert_group, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset++;
                /* Cert Count */
                proto_tree_add_item(t, hf_hip_tlv_cert_count, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset++;
                /* Cert ID */
                proto_tree_add_item(t, hf_hip_tlv_cert_id, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset++;
                /* Cert Type */
                proto_tree_add_item(t, hf_hip_tlv_cert_type, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset++;
                /* Certificate */
                proto_tree_add_item(t, hf_hip_tlv_certificate, tvb, newoffset,
                                     tlv_len-4, ENC_NA);
                break;
        case PARAM_NOTIFICATION:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_notification_res, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                /* Notification Message Type */
                proto_tree_add_item(t, hf_hip_tlv_notification_type, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                /* Notification Data */
                proto_tree_add_item(t, hf_hip_tlv_notification_data, tvb, newoffset,
                                     tlv_len-4, ENC_NA);
                break;
        case PARAM_ECHO_REQUEST_SIGNED:
        case PARAM_ECHO_RESPONSE_SIGNED:
        case PARAM_ECHO_REQUEST_UNSIGNED:
        case PARAM_ECHO_RESPONSE_UNSIGNED:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Variable length Opaque Data */
                proto_tree_add_item(t, hf_hip_tlv_opaque_data, tvb, newoffset,
                                     tlv_len, ENC_NA);
                break;
        case PARAM_REG_INFO:
        case PARAM_REG_REQUEST:
        case PARAM_REG_RESPONSE:
        case PARAM_REG_FAILED:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                if (type == PARAM_REG_INFO) {
                        /* Min Lifetime */
                        proto_tree_add_item(t, hf_hip_tlv_reg_ltmin, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        newoffset++;
                        /* Max Lifetime */
                        proto_tree_add_item(t, hf_hip_tlv_reg_ltmax, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        newoffset++;
                        tlv_len -= 2;
                } else if (type == PARAM_REG_FAILED) {
                        /* Failure Type */
                        proto_tree_add_item(t, hf_hip_tlv_reg_failtype, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        newoffset++;
                        tlv_len--;
                } else {
                        /* Lifetime */
                        proto_tree_add_item(t, hf_hip_tlv_reg_lt, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                        newoffset++;
                        tlv_len--;
                }
                /* Reg Type 1 ... n, Padding */
                while (tlv_len > 0) {
                        reg_type = tvb_get_guint8(tvb, newoffset);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                        proto_tree_add_uint_format(t, hf_hip_tlv_reg_type, tvb,
                                                   newoffset, 1, reg_type, "%u (%s)", reg_type,
                                                   val_to_str_const(reg_type, reg_type_vals, "Unknown"));
                        /* one byte per registration type */
                        tlv_len--;
                        newoffset++;
                }
                break;
        case PARAM_HMAC:
        case PARAM_HMAC_2:
        case PARAM_RVS_HMAC:
        case PARAM_RELAY_HMAC:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* HMAC */
                proto_tree_add_item(t, hf_hip_tlv_hmac, tvb, offset+4,
                                     tlv_len, ENC_NA);
                break;
        case PARAM_HIP_SIGNATURE:
        case PARAM_HIP_SIGNATURE_2:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Signature algorithm */
                n = tvb_get_guint8(tvb, offset+4);
                proto_tree_add_uint_format(t, hf_hip_tlv_sig_alg, tvb, newoffset, 1,
                                           n, "%u (%s)", n,
                                           val_to_str_const(n, sig_alg_vals, "Unknown"));
                newoffset++;
                /* Signature */
                proto_tree_add_item(t, hf_hip_tlv_sig, tvb, newoffset, tlv_len-1,
                                    ENC_NA);
                break;
        case PARAM_FROM:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Address */
                proto_tree_add_item(t, hf_hip_tlv_from_address, tvb, newoffset, 16, ENC_NA);
                break;
        case PARAM_VIA_RVS:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* RVS Addresses  */
                while (tlv_len > 0) {
                        proto_tree_add_item(t, hf_hip_tlv_rvs_address, tvb, newoffset, 16, ENC_NA);
                        tlv_len -= 16;
                        newoffset += 16;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                }
                break;
        case PARAM_RELAY_FROM:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Port */
                proto_tree_add_item(t, hf_hip_tlv_relay_from_port, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                /* Protocol */
                proto_tree_add_item(t, hf_hip_tlv_relay_from_protocol, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_relay_from_reserved, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Address */
                proto_tree_add_item(t, hf_hip_tlv_relay_from_address, tvb, newoffset, 16, ENC_NA);
                break;
        case PARAM_RELAY_TO:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Port */
                proto_tree_add_item(t, hf_hip_tlv_relay_to_port, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                /* Protocol */
                proto_tree_add_item(t, hf_hip_tlv_relay_to_protocol, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_relay_to_reserved, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Address */
                proto_tree_add_item(t, hf_hip_tlv_relay_to_address, tvb, newoffset, 16, ENC_NA);
                break;
        case PARAM_REG_FROM:
                t = proto_item_add_subtree(ti, ett_hip_tlv_data);
                /* Port */
                proto_tree_add_item(t, hf_hip_tlv_reg_from_port, tvb, newoffset, 2, ENC_BIG_ENDIAN);
                newoffset += 2;
                /* Protocol */
                proto_tree_add_item(t, hf_hip_tlv_reg_from_protocol, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Reserved */
                proto_tree_add_item(t, hf_hip_tlv_reg_from_reserved, tvb, newoffset, 1, ENC_BIG_ENDIAN);
                newoffset += 1;
                /* Address */
                proto_tree_add_item(t, hf_hip_tlv_reg_from_address, tvb, newoffset, 16, ENC_NA);
                break;
        default:
                break;
        }
        return (0);
}
