static void
CVE_2012_4287_VULN_dissect_mongo_pdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
int origin_a, origin_aa[4];

  proto_item *ti;
  proto_tree *mongo_tree;
  guint offset = 0, opcode;

  col_set_str(pinfo->cinfo, COL_PROTOCOL, "MONGO");

  if (tree) {

    ti = proto_tree_add_item(tree, proto_mongo, tvb, 0, -1, ENC_NA);

    mongo_tree = proto_item_add_subtree(ti, ett_mongo);

    proto_tree_add_item(mongo_tree, hf_mongo_message_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

    proto_tree_add_item(mongo_tree, hf_mongo_request_id, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

    proto_tree_add_item(mongo_tree, hf_mongo_response_to, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

    proto_tree_add_item(mongo_tree, hf_mongo_op_code, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    opcode = tvb_get_letohl(tvb, offset);
    offset += 4;

    if(opcode == 1)
    {
      col_set_str(pinfo->cinfo, COL_INFO, "Response :");
    }
    else
    {
      col_set_str(pinfo->cinfo, COL_INFO, "Request :");

    }
    col_append_fstr(pinfo->cinfo, COL_INFO, " %s", val_to_str(opcode, opcode_vals, "Unknown"));

    switch(opcode){
    case OP_REPLY:
      offset = dissect_mongo_reply(tvb, offset, mongo_tree);
      break;
    case OP_MSG:
      offset = dissect_mongo_msg(tvb, offset, mongo_tree);
      break;
    case OP_UPDATE:
      offset = dissect_mongo_update(tvb, offset, mongo_tree);
      break;
    case OP_INSERT:
      offset = dissect_mongo_insert(tvb, offset, mongo_tree);
      break;
    case OP_QUERY:
      offset = dissect_mongo_query(tvb, offset, mongo_tree);
      break;
    case OP_GET_MORE:
      offset = dissect_mongo_getmore(tvb, offset, mongo_tree);
      break;
    case OP_DELETE:
      offset = dissect_mongo_delete(tvb, offset, mongo_tree);
      break;
    case OP_KILL_CURSORS:
      offset = dissect_mongo_kill_cursors(tvb, offset, mongo_tree);
      break;
    default:
      /* No default Action */
      break;
    }
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    if(offset < tvb_reported_length(tvb))
    {
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
      ti = proto_tree_add_item(mongo_tree, hf_mongo_unknown, tvb, offset, -1, ENC_NA);
      expert_add_info_format(pinfo, ti, PI_UNDECODED, PI_WARN, " Unknown Data (not interpreted)");
    }
  }

}
