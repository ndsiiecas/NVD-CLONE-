static void CVE_2013_1576_PATCHED_dissect_sdp_media_attribute(tvbuff_t *tvb, packet_info *pinfo, proto_item * ti, int length, transport_info_t *transport_info) {
int judge_para = 3;
  proto_tree *sdp_media_attribute_tree, *parameter_item;
  proto_item *fmtp_item, *media_format_item, *parameter_tree;
  proto_tree *fmtp_tree;
  gint        offset, next_offset, tokenlen, n, colon_offset;
  /*??guint8 *field_name;*/
  guint8     *payload_type;
  guint8     *attribute_value;
  gint       *key;
  guint8      pt;
  gint        sdp_media_attrbute_code;
  const char *msrp_res           = "msrp://";
  const char *h324ext_h223lcparm = "h324ext/h223lcparm";
  gboolean    has_more_pars      = TRUE;
  tvbuff_t   *h245_tvb;
  guint8      master_key_length  = 0, master_salt_length = 0;
  encoding_name_and_rate_t *encoding_name_and_rate;

  offset = 0;

  /* Create attribute tree */
  sdp_media_attribute_tree = proto_item_add_subtree(ti,
                                                    ett_sdp_media_attribute);
  /* Find end of field */
  colon_offset = tvb_find_guint8(tvb, offset, -1, ':');

  if (colon_offset == -1)
    return;

  /* Attribute field name is token before ':' */
  tokenlen = colon_offset - offset;
  proto_tree_add_item(sdp_media_attribute_tree,
                      hf_media_attribute_field,
                      tvb, offset, tokenlen, ENC_ASCII|ENC_NA);
  /*??field_name = tvb_get_ephemeral_string(tvb, offset, tokenlen);*/
  sdp_media_attrbute_code = find_sdp_media_attribute_names(tvb, offset, tokenlen);

  /* Skip colon */
  offset = colon_offset + 1;
  /* skip leading wsp */
  offset = tvb_skip_wsp(tvb, offset, tvb_length_remaining(tvb, offset));

  /* Value is the remainder of the line */
  attribute_value = tvb_get_ephemeral_string(tvb, offset, tvb_length_remaining(tvb, offset));



  /*********************************************/
  /* Special parsing for some field name types */

  switch (sdp_media_attrbute_code) {
  case SDP_RTPMAP:
    /* decode the rtpmap to see if it is DynamicPayload to dissect them automatic */
    next_offset = tvb_find_guint8(tvb, offset, -1, ' ');

    if (next_offset == -1)
      return;

    tokenlen = next_offset - offset;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    proto_tree_add_item(sdp_media_attribute_tree, hf_media_format, tvb,
                        offset, tokenlen, ENC_ASCII|ENC_NA);

    payload_type = tvb_get_ephemeral_string(tvb, offset, tokenlen);

    offset = next_offset + 1;

    next_offset = tvb_find_guint8(tvb, offset, -1, '/');

    if (next_offset == -1) {
      return;
    }

    tokenlen = next_offset - offset;

    proto_tree_add_item(sdp_media_attribute_tree, hf_media_encoding_name, tvb,
                        offset, tokenlen, ENC_ASCII|ENC_NA);

    pt = atoi((char*)payload_type);
    if (pt >= SDP_NO_OF_PT) {
      return;   /* Invalid */
    }

    key  = se_alloc(sizeof (gint));
    *key = atol((char*)payload_type);

    transport_info->encoding_name[pt] = (char*)tvb_get_ephemeral_string(tvb, offset, tokenlen);

    next_offset =  next_offset + 1;
    offset = next_offset;
    while (length-1 >= next_offset) {
      if (!isdigit(tvb_get_guint8(tvb, next_offset)))
        break;
      next_offset++;
    }
    tokenlen = next_offset - offset;
    proto_tree_add_item(sdp_media_attribute_tree, hf_media_sample_rate, tvb,
                        offset, tokenlen, ENC_ASCII|ENC_NA);
    transport_info->sample_rate[pt] = atoi(tvb_get_ephemeral_string(tvb, offset, tokenlen));
    /* As per RFC2327 it is possible to have multiple Media Descriptions ("m=").
       For example:

       a=rtpmap:101 G726-32/8000
       m=audio 49170 RTP/AVP 0 97
       a=rtpmap:97 telephone-event/8000
       m=audio 49172 RTP/AVP 97 101
       a=rtpmap:97 G726-24/8000

       The Media attributes ("a="s) after the "m=" only apply for that "m=".
       If there is an "a=" before the first "m=", that attribute applies for
       all the session (all the "m="s).
    */

    /* so, if this "a=" appear before any "m=", we add it to all the dynamic
     * hash tables
     */
    if (transport_info->media_count == 0) {
      for (n = 0; n < SDP_MAX_RTP_CHANNELS; n++) {
        encoding_name_and_rate = se_alloc(sizeof (encoding_name_and_rate_t));
        encoding_name_and_rate->encoding_name = se_strdup(transport_info->encoding_name[pt]);
        encoding_name_and_rate->sample_rate = transport_info->sample_rate[pt];
        if (n == 0) {
          g_hash_table_insert(transport_info->media[n].rtp_dyn_payload,
                              key, encoding_name_and_rate);
        } else {    /* we create a new key and encoding_name to assign to the other hash tables */
          gint *key2;
          key2  = se_alloc(sizeof (gint));
          *key2 = atol((char*)payload_type);
          g_hash_table_insert(transport_info->media[n].rtp_dyn_payload,
                              key2, encoding_name_and_rate);
        }
      }
      return;
      /* if the "a=" is after an "m=", only apply to this "m=" */
    } else
      /* in case there is an overflow in SDP_MAX_RTP_CHANNELS, we keep always the last "m=" */
      encoding_name_and_rate = se_alloc(sizeof (encoding_name_and_rate_t));

    encoding_name_and_rate->encoding_name = se_strdup(transport_info->encoding_name[pt]);
    encoding_name_and_rate->sample_rate   = transport_info->sample_rate[pt];
    if (transport_info->media_count == SDP_MAX_RTP_CHANNELS-1)
      g_hash_table_insert(transport_info->media[ transport_info->media_count ].rtp_dyn_payload,
                          key, encoding_name_and_rate);
    else
      g_hash_table_insert(transport_info->media[ transport_info->media_count-1 ].rtp_dyn_payload,
                          key, encoding_name_and_rate);
    break;
  case SDP_FMTP:
    if (sdp_media_attribute_tree) {
      guint8 media_format;
      /* Reading the Format parameter(fmtp) */
      /* Skip leading space, if any */
      offset = tvb_skip_wsp(tvb, offset, tvb_length_remaining(tvb, offset));
      /* Media format extends to the next space */
      next_offset = tvb_find_guint8(tvb, offset, -1, ' ');

      if (next_offset == -1)
        return;

      tokenlen = next_offset - offset;


      media_format_item = proto_tree_add_item(sdp_media_attribute_tree,
                                              hf_media_format, tvb, offset,
                                              tokenlen, ENC_ASCII|ENC_NA);
      media_format = atoi((char*)tvb_get_ephemeral_string(tvb, offset, tokenlen));
      if (media_format >= SDP_NO_OF_PT) {
        return;   /* Invalid */
      }

      /* Append encoding name to format if known */
      proto_item_append_text(media_format_item, " [%s]",
                             transport_info->encoding_name[media_format]);

#if 0 /* XXX:  ?? */
      payload_type = tvb_get_ephemeral_string(tvb, offset, tokenlen);
#endif
      /* Move offset past the payload type */
      offset = next_offset + 1;

      while (has_more_pars == TRUE) {
        next_offset = tvb_find_guint8(tvb, offset, -1, ';');
        offset = tvb_skip_wsp(tvb, offset, tvb_length_remaining(tvb, offset));
if(judge_para * 5 > 8)  {printf("math exists!"); }

        if (next_offset == -1) {
          has_more_pars = FALSE;
          next_offset= tvb_length(tvb);
        }

        /* There are at least 2 - add the first parameter */
        tokenlen = next_offset - offset;
        fmtp_item = proto_tree_add_item(sdp_media_attribute_tree,
                                        hf_media_format_specific_parameter, tvb,
                                        offset, tokenlen, ENC_ASCII|ENC_NA);

        fmtp_tree = proto_item_add_subtree(fmtp_item, ett_sdp_fmtp);

        decode_sdp_fmtp(fmtp_tree, tvb, pinfo, offset, tokenlen,
                        transport_info->encoding_name[media_format]);

        /* Move offset past "; " and onto firts char */
        offset = next_offset + 1;
      }
    }
    break;
  case SDP_PATH:
    /* msrp attributes that contain address needed for conversation */
    /*    RFC 4975
     *    path = path-label ":" path-list
     *    path-label = "path"
     *    path-list= MSRP-URI *(SP MSRP-URI)
     *    MSRP-URI = msrp-scheme "://" authority
     *       ["/" session-id] ";" transport *( ";" URI-parameter)
     *                        ; authority as defined in RFC3986
     *
     *    msrp-scheme = "msrp" / "msrps"
     * RFC 3986
     * The authority component is preceded by a double slash ("//") and is terminated by
     * the next slash ("/"), question mark ("?"), or number sign ("#") character, or by
     * the end of the URI.
     */

    /* Check for "msrp://" */
    if (strncmp((char*)attribute_value, msrp_res, strlen(msrp_res)) == 0) {
      int address_offset, port_offset, port_end_offset;

      /* Address starts here */
      address_offset = offset + (int)strlen(msrp_res);

      /* Port is after next ':' */
      port_offset = tvb_find_guint8(tvb, address_offset, -1, ':');
      /* Check if port is present if not skipp */
      if (port_offset!= -1) {
        /* Port ends with '/' */
        port_end_offset = tvb_find_guint8(tvb, port_offset, -1, '/');

        /* Attempt to convert address */
        if (inet_pton(AF_INET,
                      (char*)tvb_get_ephemeral_string(tvb, address_offset, port_offset-address_offset),
                      &msrp_ipaddr) > 0) {
          /* Get port number */
          msrp_port_number = atoi((char*)tvb_get_ephemeral_string(tvb, port_offset + 1, port_end_offset - port_offset - 1));
          /* Set flag so this info can be used */
          msrp_transport_address_set = TRUE;
        }
      }
    }
    break;
  case SDP_H248_ITEM:
    /* Decode h248 item ITU-T Rec. H.248.12 (2001)/Amd.1 (11/2002)*/
    if (strncmp((char*)attribute_value, h324ext_h223lcparm, strlen(msrp_res)) == 0) {
      /* A.5.1.3 H.223 Logical channel parameters
       * This property indicates the H.245
       * H223LogicalChannelsParameters structure encoded by applying the PER specified in
       * ITU-T Rec. X.691. Value encoded as per A.5.1.2. For text encoding the mechanism defined
       * in ITU-T Rec. H.248.15 is used.
       */
      gint len;
      asn1_ctx_t actx;

      len = (gint)strlen(attribute_value);
      h245_tvb = ascii_bytes_to_tvb(tvb, pinfo, len, attribute_value);
      /* arbitrary maximum length */
      /* should go through a handle, however,  the two h245 entry
         points are different, one is over tpkt and the other is raw
      */
      if (h245_tvb) {
        asn1_ctx_init(&actx, ASN1_ENC_PER, TRUE, pinfo);
        dissect_h245_H223LogicalChannelParameters(h245_tvb, 0, &actx,
                                                  sdp_media_attribute_tree,
                                                  hf_SDPh223LogicalChannelParameters);
      }
    }
    break;
  case SDP_CRYPTO:
      /* http://tools.ietf.org/html/rfc4568
      * 9.1.  Generic "Crypto" Attribute Grammar
      *
      *   The ABNF grammar for the crypto attribute is defined below:
      *
      *   "a=crypto:" tag 1*WSP crypto-suite 1*WSP key-params
      *                                           *(1*WSP session-param)
      *
      *   tag              = 1*9DIGIT
      *   crypto-suite     = 1*(ALPHA / DIGIT / "_")
      *
      *   key-params       = key-param *(";" key-param)
      *   key-param        = key-method ":" key-info
      *   key-method       = "inline" / key-method-ext
      *   key-method-ext   = 1*(ALPHA / DIGIT / "_")
      *   key-info         = 1*(%x21-3A / %x3C-7E) ; visible (printing) chars
      *                                        ; except semi-colon
      *  session-param    = 1*(VCHAR)         ; visible (printing) characters
      *
      *   where WSP, ALPHA, DIGIT, and VCHAR are defined in [RFC4234].
      *
      */

      /* We are at the first colon */
      /* tag */
      next_offset = tvb_find_guint8(tvb, offset, -1, ' ');
	  if(next_offset==-1){
		  /* XXX Add expert item? */
		  return;
	  }
      tokenlen    = next_offset - offset;
      proto_tree_add_uint(sdp_media_attribute_tree, hf_sdp_crypto_tag, tvb, offset, tokenlen,
          atoi((char*)tvb_get_ephemeral_string(tvb, offset, tokenlen)));
      offset = next_offset + 1;

      /* crypto-suite */
      next_offset = tvb_find_guint8(tvb, offset, -1, ' ');
	  if(next_offset==-1){
		  /* XXX Add expert item? */
		  return;
	  }
      tokenlen    = next_offset - offset;
      proto_tree_add_item(sdp_media_attribute_tree, hf_sdp_crypto_crypto_suite,
          tvb, offset, tokenlen, ENC_ASCII|ENC_NA);
      if (tvb_strncaseeql(tvb, offset, "AES_CM_128_HMAC_SHA1_80", tokenlen) == 0) {

          /* XXX This may only work in simple cases */
          if (transport_info->encryption_algorithm == SRTP_ENC_ALG_NOT_SET) {
              transport_info->encryption_algorithm = SRTP_ENC_ALG_AES_CM;
              transport_info->auth_algorithm       = SRTP_AUTH_ALG_HMAC_SHA1;
              /* number of octets used for the Auth Tag in the RTP payload */
              transport_info->auth_tag_len         = 10;
          }
          master_key_length  = 16; /* 128 bits = 16 octets */
          master_salt_length = 14; /* 112 bits = 14 octets */
      } else if (tvb_strncaseeql(tvb, offset, "AES_CM_128_HMAC_SHA1_32", tokenlen) == 0) {
          /* XXX This may only work in simple cases */
          if (transport_info->encryption_algorithm == SRTP_ENC_ALG_NOT_SET) {
              transport_info->encryption_algorithm = SRTP_ENC_ALG_AES_CM;
              transport_info->auth_algorithm       = SRTP_AUTH_ALG_HMAC_SHA1;
              /* number of octets used for the Auth Tag in the RTP payload */
              transport_info->auth_tag_len         = 4;
          }
          master_key_length  = 16; /* 128 bits = 16 octets */
          master_salt_length = 14; /* 112 bits = 14 octets */
      } else if (tvb_strncaseeql(tvb, offset, "F8_128_HMAC_SHA1_80", tokenlen) == 0) {
          if (transport_info->encryption_algorithm == SRTP_ENC_ALG_NOT_SET) {
              /* XXX This may only work in simple cases */
              transport_info->encryption_algorithm = SRTP_ENC_ALG_AES_F8;
              transport_info->auth_algorithm       = SRTP_AUTH_ALG_HMAC_SHA1;
              /* number of octets used for the Auth Tag in the RTP payload */
              transport_info->auth_tag_len         = 10;
          }
          master_key_length  = 16; /* 128 bits = 16 octets */
          master_salt_length = 14; /* 112 bits = 14 octets */
      }
      offset = next_offset + 1;

      /* key-params */
      while (has_more_pars == TRUE) {
          int       param_end_offset;
          tvbuff_t *key_salt_tvb;
          gchar    *data_p = NULL;

          param_end_offset = tvb_find_guint8(tvb, offset, -1, ';');
          if (param_end_offset == -1) {
              has_more_pars = FALSE;
              param_end_offset = tvb_length(tvb);
          }
          /* key-method or key-method-ext */
          next_offset = tvb_find_guint8(tvb, offset, -1, ':');
          if (next_offset == -1) {
              expert_add_info_format(pinfo, parameter_item, PI_MALFORMED, PI_NOTE,
                  "Invalid key-param (no ':' delimiter)");
              break;
          }

          if (tvb_strncaseeql(tvb, offset, "inline", next_offset-offset) == 0) {
			  parameter_item = proto_tree_add_text(sdp_media_attribute_tree,
				  tvb, offset, param_end_offset-offset, "Key parameters");
			  parameter_tree = proto_item_add_subtree(parameter_item, ett_sdp_crypto_key_parameters);
              /* XXX only for SRTP? */
              /* srtp-key-info       = key-salt ["|" lifetime] ["|" mki] */
              offset      = next_offset +1;
              next_offset = tvb_find_guint8(tvb, offset, -1, '|');
              if (next_offset == -1) {
                  tokenlen = param_end_offset - offset;
              } else {
                  tokenlen = next_offset - offset;
              }
              data_p = tvb_get_ephemeral_string(tvb, offset, tokenlen);
              key_salt_tvb = base64_to_tvb(tvb, data_p);
              add_new_data_source(pinfo, key_salt_tvb, "Key_Salt_tvb");
              if (master_key_length != 0) {
                  proto_tree_add_text(parameter_tree, tvb, offset, tokenlen, "Key and Salt");
                  proto_tree_add_item(parameter_tree, hf_sdp_crypto_master_key,
                      key_salt_tvb, 0, master_key_length, ENC_ASCII|ENC_NA);
                  proto_tree_add_item(parameter_tree, hf_sdp_crypto_master_salt,
                      key_salt_tvb, master_key_length, master_salt_length, ENC_ASCII|ENC_NA);
              } else {
                  proto_tree_add_text(parameter_tree, key_salt_tvb, 0, -1, "Key and Salt");
              }

              /*  ["|" lifetime] ["|" mki] are optional */
              if (next_offset != -1) {
                  offset = next_offset + 1;
                  next_offset = tvb_find_guint8(tvb, offset, -1, '|');
                  if(next_offset == -1){
                      if(next_offset < param_end_offset){
                          next_offset = param_end_offset;
                      }
                  }
                  if (next_offset != -1) {
                      /*lifetime           = ["2^"] 1*(DIGIT)   ; see section 6.1 for "2^" */
                      tokenlen = next_offset - offset;
                      proto_tree_add_item(parameter_tree, hf_sdp_crypto_lifetime,
                          tvb, offset, tokenlen, ENC_ASCII|ENC_NA);
                      offset   = next_offset + 1;
                  }
                  /* mki                 = mki-value ":" mki-length
                  *
                  * mki-value           = 1*DIGIT
                  */
				  if(offset>param_end_offset){
					  next_offset = -1;
				  }else{
					  next_offset = tvb_find_guint8(tvb, offset, -1, ':');
				  }
                  if (next_offset != -1) {
                      tokenlen    = next_offset - offset;
                      proto_tree_add_item(parameter_tree, hf_sdp_crypto_mki, tvb, offset, tokenlen, ENC_ASCII|ENC_NA);
                      offset      = next_offset + 1;

                      /* mki-length          = 1*3DIGIT   ; range 1..128. */
                      next_offset = param_end_offset;
                      tokenlen    = next_offset - offset;

                      /* This will not work if more than one parameter */
                      /* number of octets used for the MKI in the RTP payload */
                      transport_info->mki_len = atoi((char*)tvb_get_ephemeral_string(tvb, offset, tokenlen));
                      proto_tree_add_item(parameter_tree, hf_sdp_crypto_mki_length,
                          tvb, offset, tokenlen, ENC_ASCII|ENC_NA);
                  }
              }
              offset = param_end_offset;
          } else {
			  break;
          }
      }

    break;
  default:
    /* No special treatment for values of this attribute type, just add as one item. */
    proto_tree_add_item(sdp_media_attribute_tree, hf_media_attribute_value,
                        tvb, offset, -1, ENC_ASCII|ENC_NA);
    break;
  }
}
