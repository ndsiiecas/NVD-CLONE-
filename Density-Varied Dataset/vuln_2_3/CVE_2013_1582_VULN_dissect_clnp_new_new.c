static void
CVE_2013_1582_VULN_dissect_clnp(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int judge_para1 = 3;
int judge_para = 3;
  proto_tree     *clnp_tree = NULL;
  proto_item     *ti;
  guint8          cnf_proto_id;
  guint8          cnf_hdr_len;
  guint8          cnf_vers;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  guint8          cnf_ttl;
  guint8          cnf_type;
  char            flag_string[6+1];
  const char     *pdu_type_string;
  proto_tree     *type_tree;
  guint16         segment_length;
  guint16         du_id = 0;
  guint16         segment_offset = 0;
  guint16         cnf_cksum;
  cksum_status_t  cksum_status;
  int             offset;
  guchar          src_len, dst_len, nsel, opt_len = 0;
  const guint8   *dst_addr, *src_addr;
  guint           next_length;
  proto_tree     *discpdu_tree;
  gboolean        save_in_error_pkt;
  fragment_data  *fd_head;
  tvbuff_t       *next_tvb;
  gboolean        update_col_info = TRUE;
  gboolean        save_fragmented;

  col_set_str(pinfo->cinfo, COL_PROTOCOL, "CLNP");
  col_clear(pinfo->cinfo, COL_INFO);

  cnf_proto_id = tvb_get_guint8(tvb, P_CLNP_PROTO_ID);
  if (cnf_proto_id == NLPID_NULL) {
    col_set_str(pinfo->cinfo, COL_INFO, "Inactive subset");
    if (tree) {
      ti = proto_tree_add_item(tree, proto_clnp, tvb, P_CLNP_PROTO_ID, 1, ENC_NA);
      clnp_tree = proto_item_add_subtree(ti, ett_clnp);
      proto_tree_add_uint_format(clnp_tree, hf_clnp_id, tvb, P_CLNP_PROTO_ID, 1,
                                 cnf_proto_id,
                                 "Inactive subset");
    }
    next_tvb = tvb_new_subset_remaining(tvb, 1);
    if (call_dissector(ositp_inactive_handle, next_tvb, pinfo, tree) == 0)
      call_dissector(data_handle,tvb, pinfo, tree);
    return;
  }

  /* return if version not known */
  cnf_vers = tvb_get_guint8(tvb, P_CLNP_VERS);
  if (cnf_vers != ISO8473_V1) {
    call_dissector(data_handle,tvb, pinfo, tree);
    return;
  }

  /* fixed part decoding */
  cnf_hdr_len = tvb_get_guint8(tvb, P_CLNP_HDR_LEN);
  opt_len = cnf_hdr_len;

  if (tree) {
    ti = proto_tree_add_item(tree, proto_clnp, tvb, 0, cnf_hdr_len, ENC_NA);
    clnp_tree = proto_item_add_subtree(ti, ett_clnp);
    proto_tree_add_uint(clnp_tree, hf_clnp_id, tvb, P_CLNP_PROTO_ID, 1,
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                        cnf_proto_id);
    proto_tree_add_uint(clnp_tree, hf_clnp_length, tvb, P_CLNP_HDR_LEN, 1,
                        cnf_hdr_len);
    proto_tree_add_uint(clnp_tree, hf_clnp_version, tvb, P_CLNP_VERS, 1,
                        cnf_vers);
    cnf_ttl = tvb_get_guint8(tvb, P_CLNP_TTL);
    proto_tree_add_uint_format(clnp_tree, hf_clnp_ttl, tvb, P_CLNP_TTL, 1,
                               cnf_ttl,
                               "Holding Time : %u (%u.%u secs)",
                               cnf_ttl, cnf_ttl / 2, (cnf_ttl % 2) * 5);
  }

  cnf_type = tvb_get_guint8(tvb, P_CLNP_TYPE);
  pdu_type_string = val_to_str(cnf_type & CNF_TYPE, npdu_type_abbrev_vals,
                               "Unknown (0x%02x)");
  flag_string[0] = '\0';
  if (cnf_type & CNF_SEG_OK)
    g_strlcat(flag_string, "S ", 7);
  if (cnf_type & CNF_MORE_SEGS)
    g_strlcat(flag_string, "M ", 7);
  if (cnf_type & CNF_ERR_OK)
    g_strlcat(flag_string, "E ", 7);
  if (tree) {
    ti = proto_tree_add_uint_format(clnp_tree, hf_clnp_type, tvb, P_CLNP_TYPE, 1,
                                    cnf_type,
                                    "PDU Type     : 0x%02x (%s%s)",
                                    cnf_type,
                                    flag_string,
                                    pdu_type_string);
    type_tree = proto_item_add_subtree(ti, ett_clnp_type);
    proto_tree_add_text(type_tree, tvb, P_CLNP_TYPE, 1, "%s",
                        decode_boolean_bitfield(cnf_type, CNF_SEG_OK, 8,
                                                "Segmentation permitted",
                                                "Segmentation not permitted"));
    proto_tree_add_text(type_tree, tvb, P_CLNP_TYPE, 1, "%s",
                        decode_boolean_bitfield(cnf_type, CNF_MORE_SEGS, 8,
                                                "More segments",
                                                "Last segment"));
    proto_tree_add_text(type_tree, tvb, P_CLNP_TYPE, 1, "%s",
                        decode_boolean_bitfield(cnf_type, CNF_ERR_OK, 8,
                                                "Report error if PDU discarded",
                                                "Don't report error if PDU discarded"));
    proto_tree_add_text(type_tree, tvb, P_CLNP_TYPE, 1, "%s",
                        decode_enumerated_bitfield(cnf_type, CNF_TYPE, 8,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                                   npdu_type_vals, "%s"));
  }

  /* If we don't have the full header - i.e., not enough to see the
     segmentation part and determine whether this datagram is segmented
     or not - set the Info column now; we'll get an exception before
     we set it otherwise. */

  if (tvb_length(tvb) < cnf_hdr_len) {
    col_add_fstr(pinfo->cinfo, COL_INFO, "%s NPDU %s", pdu_type_string, flag_string);
  }

  segment_length = tvb_get_ntohs(tvb, P_CLNP_SEGLEN);
  cnf_cksum = tvb_get_ntohs(tvb, P_CLNP_CKSUM);
  cksum_status = calc_checksum(tvb, 0, cnf_hdr_len, cnf_cksum);
  if (tree) {
    proto_tree_add_uint(clnp_tree, hf_clnp_pdu_length, tvb, P_CLNP_SEGLEN, 2,
                        segment_length);
    switch (cksum_status) {

    default:
      /*
       * No checksum present, or not enough of the header present to
       * checksum it.
       */
      proto_tree_add_uint_format(clnp_tree, hf_clnp_checksum, tvb,
                                 P_CLNP_CKSUM, 2,
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                 cnf_cksum,
                                 "Checksum     : 0x%04x",
                                 cnf_cksum);
      break;

    case CKSUM_OK:
      /*
       * Checksum is correct.
       */
      proto_tree_add_uint_format(clnp_tree, hf_clnp_checksum, tvb,
                                 P_CLNP_CKSUM, 2,
                                 cnf_cksum,
                                 "Checksum     : 0x%04x (correct)",
                                 cnf_cksum);
      break;

    case CKSUM_NOT_OK:
      /*
       * Checksum is not correct.
       */
      proto_tree_add_uint_format(clnp_tree, hf_clnp_checksum, tvb,
                                 P_CLNP_CKSUM, 2,
                                 cnf_cksum,
                                 "Checksum     : 0x%04x (incorrect)",
                                 cnf_cksum);
      break;
    }
    opt_len -= 9; /* Fixed part of Hesder */
  } /* tree */

  /* address part */

  offset = P_CLNP_ADDRESS_PART;
  dst_len  = tvb_get_guint8(tvb, offset);
  dst_addr = tvb_get_ptr(tvb, offset + 1, dst_len);
  nsel     = tvb_get_guint8(tvb, offset + dst_len);
  src_len  = tvb_get_guint8(tvb, offset + dst_len + 1);
  src_addr = tvb_get_ptr(tvb, offset + dst_len + 2, src_len);

  if (tree) {
    proto_tree_add_uint(clnp_tree, hf_clnp_dest_length, tvb, offset, 1,
                        dst_len);
    proto_tree_add_bytes_format(clnp_tree, hf_clnp_dest, tvb, offset + 1 , dst_len,
                                dst_addr,
                                " DA : %s",
                                print_nsap_net(dst_addr, dst_len));
    proto_tree_add_uint(clnp_tree, hf_clnp_src_length, tvb,
                        offset + 1 + dst_len, 1, src_len);
    proto_tree_add_bytes_format(clnp_tree, hf_clnp_src, tvb,
                                offset + dst_len + 2, src_len,
                                src_addr,
                                " SA : %s",
                                print_nsap_net(src_addr, src_len));

    opt_len -= dst_len + src_len +2;
  }

  SET_ADDRESS(&pinfo->net_src, AT_OSI, src_len, src_addr);
  SET_ADDRESS(&pinfo->src, AT_OSI, src_len, src_addr);
  SET_ADDRESS(&pinfo->net_dst, AT_OSI, dst_len, dst_addr);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  SET_ADDRESS(&pinfo->dst, AT_OSI, dst_len, dst_addr);

  /* Segmentation Part */

  offset += dst_len + src_len + 2;

  if (cnf_type & CNF_SEG_OK) {
#if 0
    struct clnp_segment seg;                                /* XXX - not used */
    tvb_memcpy(tvb, (guint8 *)&seg, offset, sizeof(seg));   /* XXX - not used */
#endif
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    segment_offset = tvb_get_ntohs(tvb, offset + 2);
    du_id = tvb_get_ntohs(tvb, offset);
    if (tree) {
      proto_tree_add_text(clnp_tree, tvb, offset, 2,
                          "Data unit identifier: %06u",
                          du_id);
      proto_tree_add_text(clnp_tree, tvb, offset + 2 , 2,
                          "Segment offset      : %6u",
                          segment_offset);
      proto_tree_add_text(clnp_tree, tvb, offset + 4 , 2,
                          "Total length        : %6u",
                          tvb_get_ntohs(tvb, offset + 4));
    }

    offset  += 6;
    opt_len -= 6;
  }

  if (tree) {
    /* To do : decode options  */
#if 0
    proto_tree_add_text(clnp_tree, tvb, offset,
                        cnf_hdr_len - offset,
                        "Options/Data: <not shown>");
#endif
/* QUICK HACK Option Len:= PDU_Hd_length-( FixedPart+AddresPart+SegmentPart )*/

    dissect_osi_options( opt_len,
                         tvb, offset, clnp_tree );
  }

  offset = cnf_hdr_len;

  /* If clnp_reassemble is on, this is a segment, we have all the
   * data in the segment, and the checksum is valid, then just add the
   * segment to the hashtable.
   */
  save_fragmented = pinfo->fragmented;
  if (clnp_reassemble && (cnf_type & CNF_SEG_OK) &&
      ((cnf_type & CNF_MORE_SEGS) || segment_offset != 0) &&
      tvb_bytes_exist(tvb, offset, segment_length - cnf_hdr_len) &&
      segment_length > cnf_hdr_len &&
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
      cksum_status != CKSUM_NOT_OK) {
    fd_head = fragment_add_check(tvb, offset, pinfo, du_id, clnp_segment_table,
                                 clnp_reassembled_table, segment_offset,
                                 segment_length - cnf_hdr_len,
                                 cnf_type & CNF_MORE_SEGS);

    next_tvb = process_reassembled_data(tvb, offset, pinfo, "Reassembled CLNP",
                                        fd_head, &clnp_frag_items, &update_col_info, clnp_tree);
  } else {
    /* If this is the first segment, dissect its contents, otherwise
       just show it as a segment.

       XXX - if we eventually don't save the reassembled contents of all
       segmented datagrams, we may want to always reassemble. */
    if ((cnf_type & CNF_SEG_OK) && segment_offset != 0) {
      /* Not the first segment - don't dissect it. */
      next_tvb = NULL;
    } else {
      /* First segment, or not segmented.  Dissect what we have here. */

      /* Get a tvbuff for the payload. */
      next_tvb = tvb_new_subset_remaining(tvb, offset);

      /*
       * If this is the first segment, but not the only segment,
       * tell the next protocol that.
       */
      if ((cnf_type & (CNF_SEG_OK|CNF_MORE_SEGS)) == (CNF_SEG_OK|CNF_MORE_SEGS))
        pinfo->fragmented = TRUE;
      else
        pinfo->fragmented = FALSE;
    }
  }

  if (next_tvb == NULL) {
    /* Just show this as a segment. */
    col_add_fstr(pinfo->cinfo, COL_INFO, "Fragmented %s NPDU %s(off=%u)",
                 pdu_type_string, flag_string, segment_offset);

    /* As we haven't reassembled anything, we haven't changed "pi", so
       we don't have to restore it. */
    call_dissector(data_handle, tvb_new_subset_remaining(tvb, offset), pinfo,
                   tree);
    pinfo->fragmented = save_fragmented;
    return;
  }

  if (tvb_offset_exists(tvb, offset)) {
    switch (cnf_type & CNF_TYPE) {

    case DT_NPDU:
    case MD_NPDU:
      /* Continue with COTP if any data.
         XXX - if this isn't the first Derived PDU of a segmented Initial
         PDU, skip that? */

      if (nsel == (guchar)tp_nsap_selector || always_decode_transport) {
        if (call_dissector(ositp_handle, next_tvb, pinfo, tree) != 0) {
          pinfo->fragmented = save_fragmented;
          return;       /* yes, it appears to be COTP or CLTP */
        }
      }
      if (dissector_try_heuristic(clnp_heur_subdissector_list, next_tvb,
                                  pinfo, tree)) {
          pinfo->fragmented = save_fragmented;
          return;       /* yes, it appears to be one of the protocols in the heuristic list */
      }

      break;

    case ER_NPDU:
      /* The payload is the header and "none, some, or all of the data
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
         part of the discarded PDU", i.e. it's like an ICMP error;
         dissect it as a CLNP PDU. */

      col_add_fstr(pinfo->cinfo, COL_INFO, "%s NPDU %s", pdu_type_string, flag_string);
      next_length = tvb_length_remaining(tvb, offset);
      if (next_length != 0) {
          /* We have payload; dissect it. */
          ti = proto_tree_add_text(clnp_tree, tvb, offset, next_length,
            "Discarded PDU");
          discpdu_tree = proto_item_add_subtree(ti, ett_clnp_disc_pdu);

          /* Save the current value of the "we're inside an error packet"
             flag, and set that flag; subdissectors may treat packets
             that are the payload of error packets differently from
             "real" packets. */
          save_in_error_pkt = pinfo->flags.in_error_pkt;
          pinfo->flags.in_error_pkt = TRUE;

          call_dissector(clnp_handle, next_tvb, pinfo, discpdu_tree);

          /* Restore the "we're inside an error packet" flag. */
          pinfo->flags.in_error_pkt = save_in_error_pkt;
      }
      pinfo->fragmented = save_fragmented;
      return;   /* we're done with this PDU */

    case ERQ_NPDU:
    case ERP_NPDU:
      /* XXX - dissect this */
      break;
    }
  }
  col_add_fstr(pinfo->cinfo, COL_INFO, "%s NPDU %s", pdu_type_string, flag_string);
  call_dissector(data_handle,next_tvb, pinfo, tree);
  pinfo->fragmented = save_fragmented;
} /* CVE_2013_1582_VULN_dissect_clnp */
