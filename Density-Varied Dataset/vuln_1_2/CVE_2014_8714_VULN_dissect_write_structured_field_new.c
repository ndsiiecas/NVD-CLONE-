static guint32
CVE_2014_8714_VULN_dissect_write_structured_field(proto_tree *tn5250_tree, tvbuff_t *tvb, gint offset)
{
int judge_para = 3;
  int start = offset;
  guint16 sf_length = 0;
  int length, type, done = 0, used = 0;

  hf_items standard_fields[] = {
    { &hf_tn5250_sf_length, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_sf_class, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_sf_type, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *qss_byte1[] = {
    &hf_tn5250_wsf_qss_flag1_0,
    &hf_tn5250_wsf_qss_flag1_reserved,
    NULL
  };

  static const int *qss_byte2[] = {
    &hf_tn5250_wsf_qss_flag2_reserved,
    &hf_tn5250_wsf_qss_flag2_7,
    NULL
  };

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  hf_items qss_fields[] = {
    { &hf_tn5250_wsf_qss_flag1, ett_tn5250_wsf_qss_mask, 1, qss_byte1, 0 },
    { &hf_tn5250_wsf_qss_flag2, ett_tn5250_wsf_qss_mask, 1, qss_byte2, 0 },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dawt_fields[] = {
    { &hf_tn5250_dawt_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dawt_char, 0, 1, 0, ENC_EBCDIC|ENC_NA },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dckf_fields[] = {
    { &hf_tn5250_dckf_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dckf_key_code, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dckf_function_code, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *rts_byte1[] = {
    &hf_tn5250_rts_flag1_0,
    &hf_tn5250_rts_flag1_reserved,
    NULL
  };

  hf_items rts_fields[] = {
    { &hf_tn5250_rts_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_rts_flag1, ett_tn5250_wsf_rts_mask, 1, rts_byte1, 0 },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *dpo_byte1[] = {
    &hf_tn5250_dpo_flag1_0,
    &hf_tn5250_dpo_flag1_1,
    &hf_tn5250_dpo_flag1_2,
    &hf_tn5250_dpo_flag1_3,
    &hf_tn5250_dpo_flag1_4,
    &hf_tn5250_dpo_flag1_5,
    &hf_tn5250_dpo_flag1_6,
    &hf_tn5250_dpo_flag1_7,
    NULL
  };

  static const int *dpo_byte2[] = {
    &hf_tn5250_dpo_flag2_0,
    &hf_tn5250_dpo_flag2_reserved,
    NULL
  };

  hf_items dpo_fields[] = {
    { &hf_tn5250_dpo_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dpo_flag1, ett_tn5250_wsf_dpo_mask, 1, dpo_byte1, 0 },
    { &hf_tn5250_dpo_flag2, ett_tn5250_wsf_dpo_mask, 1, dpo_byte2, 0 },
    { &hf_tn5250_dpo_displace_characters, 0, 3, 0, ENC_EBCDIC|ENC_NA },
    { &hf_tn5250_dpo_start_location_row, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dpo_start_location_col, 0, 2, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *dtsf_byte1[] = {
    &hf_tn5250_dtsf_flag1_0,
    &hf_tn5250_dtsf_flag1_1,
    &hf_tn5250_dtsf_flag1_2,
    &hf_tn5250_dtsf_flag1_3,
    &hf_tn5250_dtsf_flag1_4,
    &hf_tn5250_dtsf_flag1_5,
    &hf_tn5250_dtsf_flag1_6,
    &hf_tn5250_dtsf_flag1_7,
    NULL
  };

  static const int *dtsf_byte2[] = {
    &hf_tn5250_dtsf_flag2_0,
    &hf_tn5250_dtsf_flag2_1,
    &hf_tn5250_dtsf_flag2_2,
    &hf_tn5250_dtsf_flag2_3,
    &hf_tn5250_dtsf_flag2_4to7,
    NULL
  };

  hf_items dtsf_fields[] = {
    { &hf_tn5250_dtsf_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dtsf_flag1, ett_tn5250_wsf_dtsf_mask, 1, dtsf_byte1, 0 },
    { &hf_tn5250_dtsf_flag2, ett_tn5250_wsf_dtsf_mask, 1, dtsf_byte2, 0 },
    { &hf_tn5250_dtsf_text_body_height, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dtsf_text_body_width, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dtsf_line_cmd_field_size, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dtsf_location_of_pitch, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dtsf_first_line, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *dsl_byte1[] = {
    &hf_tn5250_dsl_flag1_0,
    &hf_tn5250_dsl_flag1_1,
    &hf_tn5250_dsl_flag1_2,
    &hf_tn5250_dsl_flag1_reserved,
    NULL
  };

  hf_items dsl_fields[] = {
    { &hf_tn5250_dsl_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsl_rtl_offset, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsl_offset, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dsl_fields2[] = {
    { &hf_tn5250_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsl_flag1, ett_tn5250_wsf_dsl_mask, 1, dsl_byte1, 0 },
    { &hf_tn5250_dsl_id, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsl_location, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsl_function, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *wts_byte1[] = {
    &hf_tn5250_wts_flag1_0,
    &hf_tn5250_wts_flag1_1,
    &hf_tn5250_wts_flag1_2,
    &hf_tn5250_wts_flag1_3,
    &hf_tn5250_wts_flag1_reserved,
    NULL
  };

  static const int *wts_byte2[] = {
    &hf_tn5250_wts_flag2_reserved,
    &hf_tn5250_wts_flag2_6,
    &hf_tn5250_wts_flag2_reserved2,
    NULL
  };

  static const int *wts_byte3[] = {
    &hf_tn5250_wts_flag3_0,
    &hf_tn5250_wts_flag3_1,
    &hf_tn5250_wts_flag3_2,
    &hf_tn5250_wts_flag3_3,
    &hf_tn5250_wts_flag3_4,
    &hf_tn5250_wts_flag3_5,
    &hf_tn5250_wts_flag3_6,
    &hf_tn5250_wts_flag3_7,
    NULL
  };

  hf_items wts_fields[] = {
    { &hf_tn5250_wts_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_flag1, ett_tn5250_wts_mask, 1, wts_byte1, 0 },
    { &hf_tn5250_wts_flag2, ett_tn5250_wts_mask, 1, wts_byte2, 0 },
    { &hf_tn5250_wts_flag3, ett_tn5250_wts_mask, 1, wts_byte3, 0 },
    { &hf_tn5250_wts_home_position_row, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_home_position_col, 0, 2, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  static const int *wts_cld_byte1[] = {
    &hf_tn5250_wts_cld_flag1_0,
    &hf_tn5250_wts_cld_flag1_1,
    &hf_tn5250_wts_cld_flag1_2,
    &hf_tn5250_wts_cld_flag1_3,
    &hf_tn5250_wts_cld_flag1_4,
    &hf_tn5250_wts_cld_flag1_5,
    &hf_tn5250_wts_cld_flag1_6,
    &hf_tn5250_wts_cld_flag1_7,
    NULL
  };

  static const int *wts_cld_byte2[] = {
    &hf_tn5250_wts_cld_flag2_0,
    &hf_tn5250_wts_cld_flag2_1,
    &hf_tn5250_wts_cld_flag2_2,
    &hf_tn5250_wts_cld_flag2_3,
    &hf_tn5250_wts_cld_flag2_4,
    &hf_tn5250_wts_cld_flag2_line_spacing,
    NULL
  };

  static const int *wts_cld_byte3[] = {
    &hf_tn5250_wts_cld_flag3_0,
    &hf_tn5250_wts_cld_flag3_1,
    &hf_tn5250_wts_cld_flag3_2,
    &hf_tn5250_wts_cld_flag3_3,
    &hf_tn5250_wts_cld_flag3_4,
    &hf_tn5250_wts_cld_flag3_5,
    &hf_tn5250_wts_cld_flag3_6,
    &hf_tn5250_wts_cld_flag3_7,
    NULL
  };

  hf_items wts_line_data_fields[] = {
    { &hf_tn5250_length, 0, 1, 0, ENC_BIG_ENDIAN }, /*FIXME: Could be one or two bytes! */
    { &hf_tn5250_wts_cld_flag1, ett_tn5250_wts_mask, 1, wts_cld_byte1, 0 },
    { &hf_tn5250_wts_cld_flag2, ett_tn5250_wts_mask, 1, wts_cld_byte2, 0 },
    { &hf_tn5250_wts_cld_row, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_cld_flag3, ett_tn5250_wts_mask, 1, wts_cld_byte3, 0 },
    { &hf_tn5250_wts_cld_page_num, 0, 2, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_cld_lmo, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_cld_io, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_wts_cld_sli, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };


  hf_items dsc_fields[] = {
    { &hf_tn5250_dsc_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsc_sk, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dsc_ev, 0, 1, 0, ENC_EBCDIC|ENC_NA },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dorm_fields[] = {
    { &hf_tn5250_dorm_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dorm_ec, 0, 2, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };


  static const int *dfdpck_coreflag[] = {
    &hf_tn5250_dfdpck_coreflag_0,
    &hf_tn5250_dfdpck_coreflag_1,
    &hf_tn5250_dfdpck_coreflag_2,
    &hf_tn5250_dfdpck_coreflag_3,
    &hf_tn5250_dfdpck_coreflag_4,
    &hf_tn5250_dfdpck_coreflag_5,
    &hf_tn5250_dfdpck_coreflag_6,
    &hf_tn5250_dfdpck_coreflag_7,
    NULL
  };

  static const int *dfdpck_toprowflag1[] = {
    &hf_tn5250_dfdpck_toprowflag1_0,
    &hf_tn5250_dfdpck_toprowflag1_1,
    &hf_tn5250_dfdpck_toprowflag1_2,
    &hf_tn5250_dfdpck_toprowflag1_3,
    &hf_tn5250_dfdpck_toprowflag1_4,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    &hf_tn5250_dfdpck_toprowflag1_5,
    &hf_tn5250_dfdpck_toprowflag1_6,
    &hf_tn5250_dfdpck_toprowflag1_7,
    NULL
  };

  static const int *dfdpck_toprowflag2[] = {
    &hf_tn5250_dfdpck_toprowflag2_0,
    &hf_tn5250_dfdpck_toprowflag2_1,
    &hf_tn5250_dfdpck_toprowflag2_2,
    &hf_tn5250_dfdpck_toprowflag2_3,
    &hf_tn5250_dfdpck_toprowflag2_4,
    &hf_tn5250_dfdpck_toprowflag2_5,
    &hf_tn5250_dfdpck_toprowflag2_6,
    &hf_tn5250_dfdpck_toprowflag2_7,
    NULL
  };

  static const int *dfdpck_toprowflag3[] = {
    &hf_tn5250_dfdpck_toprowflag3_0,
    &hf_tn5250_dfdpck_toprowflag3_1,
    &hf_tn5250_dfdpck_toprowflag3_2,
    &hf_tn5250_dfdpck_toprowflag3_3,
    &hf_tn5250_dfdpck_toprowflag3_4,
    &hf_tn5250_dfdpck_toprowflag3_5,
    &hf_tn5250_dfdpck_toprowflag3_6,
    &hf_tn5250_dfdpck_toprowflag3_7,
    NULL
  };

  hf_items dfdpck_fields[] = {
    { &hf_tn5250_dfdpck_partition, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dfdpck_core_area_fields[] = {
    { &hf_tn5250_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dfdpck_data_field, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dfdpck_coreflag, ett_tn5250_dfdpck_mask, 1, dfdpck_coreflag, 0 },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  hf_items dfdpck_top_row_fields[] = {
    { &hf_tn5250_length, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dfdpck_data_field, 0, 1, 0, ENC_BIG_ENDIAN },
    { &hf_tn5250_dfdpck_toprowflag1, ett_tn5250_dfdpck_mask, 1, dfdpck_toprowflag1, 0 },
    { &hf_tn5250_dfdpck_toprowflag2, ett_tn5250_dfdpck_mask, 1, dfdpck_toprowflag2, 0 },
    { &hf_tn5250_dfdpck_toprowflag3, ett_tn5250_dfdpck_mask, 1, dfdpck_toprowflag3, 0 },
    { &hf_tn5250_reserved, 0, 1, 0, ENC_BIG_ENDIAN },
    { NULL, 0, 0, 0, 0 }
  };

  while (tvb_reported_length_remaining(tvb, offset) > 0 && !done) {
    sf_length = tvb_get_ntohs(tvb,offset);
    type = tvb_get_guint8(tvb, offset+3);

    offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, standard_fields);

    switch (type) {
      case PASS_THROUGH:
        proto_tree_add_item(tn5250_tree, hf_tn5250_field_data, tvb, offset,
                            (sf_length - (start + offset)), ENC_EBCDIC|ENC_NA);
        offset += (guint32)(sf_length - (start + offset));
        break;
      case TN5250_QUERY:
        proto_tree_add_item(tn5250_tree, hf_tn5250_reserved, tvb, offset,
                            1, ENC_BIG_ENDIAN);
        offset ++;
        break;
      case TN5250_QUERY_STATION_STATE:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, qss_fields);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        break;
      case DEFINE_AUDIT_WINDOW__TABLE:
        proto_tree_add_item(tn5250_tree, hf_tn5250_dawt_id, tvb, offset,
                            1, ENC_BIG_ENDIAN);
        while ((offset - start) < sf_length) {
          length = tvb_get_guint8(tvb,offset);
          offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, dawt_fields);
          proto_tree_add_item(tn5250_tree, hf_tn5250_dawt_message, tvb, offset,
                              (length - 2), ENC_EBCDIC|ENC_NA);
          offset += length;
        }
        break;
      case DEFINE_COMMAND_KEY_FUNCTION:
        proto_tree_add_item(tn5250_tree, hf_tn5250_dckf_id, tvb, offset,
                            1, ENC_BIG_ENDIAN);
        while ((offset - start) < sf_length) {
          length = tvb_get_guint8(tvb,offset);
          offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                        dckf_fields);
          proto_tree_add_item(tn5250_tree, hf_tn5250_dckf_prompt_text, tvb,
                              offset, (length - 2), ENC_EBCDIC|ENC_NA);
          offset += length;
        }
        break;
      case READ_TEXT_SCREEN:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, rts_fields);
        break;
      case DEFINE_PENDING_OPERATIONS:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, dpo_fields);
        break;
      case DEFINE_TEXT_SCREEN_FORMAT:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, dtsf_fields);
        break;
      case DEFINE_SCALE_LINE:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, dsl_fields);
        while ((offset - start) < sf_length) {
          /* XXX length unused
          length = tvb_get_guint8(tvb,offset); */
          offset += tn5250_add_hf_items(tn5250_tree, tvb, offset, dsl_fields2);
        }
        break;
      case WRITE_TEXT_SCREEN:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                      wts_fields);
        length = tvb_get_guint8(tvb,offset);
        used = tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                   wts_line_data_fields);
        offset += used;
        proto_tree_add_item(tn5250_tree, hf_tn5250_wts_cld_li, tvb, offset,
                            (length - used), ENC_EBCDIC|ENC_NA);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        break;
      case DEFINE_SPECIAL_CHARACTERS:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                      dsc_fields);
        break;
      case DEFINE_OPERATOR_ERROR_MESSAGES:
        proto_tree_add_item(tn5250_tree, hf_tn5250_dorm_id, tvb, offset,
                            1, ENC_BIG_ENDIAN);
        while ((offset - start) < sf_length) {
          length = tvb_get_guint8(tvb,offset);
          offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                        dorm_fields);
          proto_tree_add_item(tn5250_tree, hf_tn5250_dorm_mt, tvb, offset,
                              (length - 2), ENC_EBCDIC|ENC_NA);
          offset += length;
        }
        break;
      case DEFINE_PITCH_TABLE:
        proto_tree_add_item(tn5250_tree, hf_tn5250_dpt_id, tvb, offset,
                            1, ENC_BIG_ENDIAN);
        while ((offset - start) < sf_length) {
          length = tvb_get_guint8(tvb,offset);
          proto_tree_add_item(tn5250_tree, hf_tn5250_length, tvb, offset,
                              1, ENC_BIG_ENDIAN);
          proto_tree_add_item(tn5250_tree, hf_tn5250_dpt_ec, tvb, offset,
                              length, ENC_EBCDIC|ENC_NA);
          offset += length;
        }
        break;
      case DEFINE_FAKE_DP_COMMAND_KEY_FUNCTION:
        offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                      dfdpck_fields);
        while ((offset - start) < sf_length) {
          length = tvb_get_guint8(tvb,offset);
          type = tvb_get_guint8(tvb,offset+1);
          if (type == CORE_AREA_COMMAND_KEYS) {
            offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                          dfdpck_core_area_fields);
          } else if (type == TOP_ROW_COMMAND_KEYS) {
            offset += tn5250_add_hf_items(tn5250_tree, tvb, offset,
                                          dfdpck_top_row_fields);
          } else {
            offset += dissect_unknown_data(tn5250_tree, tvb, offset, start, length);
          }
        }
        break;
      default:
        done = 1;
        break;
    }
  }

  offset += dissect_unknown_data(tn5250_tree, tvb, offset, start, sf_length);

  return (offset - start);
}
