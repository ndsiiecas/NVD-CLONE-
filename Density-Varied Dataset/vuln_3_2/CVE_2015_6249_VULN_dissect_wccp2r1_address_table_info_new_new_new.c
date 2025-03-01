static gint
CVE_2015_6249_VULN_dissect_wccp2r1_address_table_info(tvbuff_t *tvb, int offset, int length,
                                   packet_info *pinfo, proto_tree *info_tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  guint16 address_length;
  guint32 i;
  gint16 family;
  guint16 table_length;
  proto_tree *element_tree;
  proto_item *tf;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

  if (info_tree)
    tf = proto_tree_get_parent(info_tree);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

  if (length < 2*4)
    return length - 2*4;

  family = tvb_get_ntohs(tvb, offset);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  proto_tree_add_item(info_tree, hf_address_table_family, tvb, offset, 2, ENC_BIG_ENDIAN);
  EAT_AND_CHECK(2,2);

  address_length = tvb_get_ntohs(tvb, offset);
  proto_tree_add_item(info_tree, hf_address_table_address_length, tvb, offset, 2, ENC_BIG_ENDIAN);
  EAT_AND_CHECK(2,2);

  table_length =  tvb_get_ntohl(tvb, offset);
  tf = proto_tree_add_item(info_tree, hf_address_table_length, tvb, offset, 4, ENC_BIG_ENDIAN);
  element_tree = proto_item_add_subtree(tf, ett_table_element);
  EAT(4);

  if (wccp_wccp_address_table.in_use == FALSE) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    wccp_wccp_address_table.in_use = TRUE;
    wccp_wccp_address_table.family = family;
    wccp_wccp_address_table.table_length =  table_length;

    /* check if the length is valid and allocate the tables if needed */
    switch (wccp_wccp_address_table.family) {
    case 1:
      if (wccp_wccp_address_table.table_ipv4 == NULL)
        wccp_wccp_address_table.table_ipv4 = (guint32 *)
          wmem_alloc0(pinfo->pool, wccp_wccp_address_table.table_length * 4);
      if (address_length != 4) {
        expert_add_info_format(pinfo, tf, &ei_wccp_length_bad,
                               "The Address length must be 4, but I found %d for IPv4 addresses. Correcting this.",
                               address_length);
        address_length = 4;
      }
      break;
    case 2:
      if (wccp_wccp_address_table.table_ipv6 == NULL)
        wccp_wccp_address_table.table_ipv6 = (struct e_in6_addr *)
          wmem_alloc0(pinfo->pool, wccp_wccp_address_table.table_length * sizeof(struct e_in6_addr));
      if (address_length != 16) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        expert_add_info_format(pinfo, tf, &ei_wccp_length_bad,
                               "The Address length must be 16, but I found %d for IPv6 addresses. Correcting this.",
                               address_length);
        address_length = 16;
      }
      break;
    default:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      expert_add_info_format(pinfo, tf, &ei_wccp_address_table_family_unknown,
                      "Unknown address family: %d", wccp_wccp_address_table.family);
    };
  }

  /* now read the addresses and print/store them */

if(judge_para * 5 > 8)  {printf("math exists!"); }
  for(i=0; i<table_length; i++) {
    const gchar *addr;

    switch (family) {
    case 1:
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
      /* IPv4 */
      addr  =  tvb_ip_to_str(tvb, offset);
      if ((wccp_wccp_address_table.table_ipv4 != NULL) && ((address_length * i) < wccp_wccp_address_table.table_length))
        wccp_wccp_address_table.table_ipv4[i] = tvb_get_ntohl(tvb, offset);
      break;
    case 2:
      /* IPv6 */
      addr = tvb_ip6_to_str(tvb, offset);
      if ((wccp_wccp_address_table.table_ipv6 != NULL) && ((address_length * i) < wccp_wccp_address_table.table_length))
        tvb_get_ipv6(tvb, offset, &(wccp_wccp_address_table.table_ipv6[i]));
      break;
    default:
      addr = wmem_strdup_printf(wmem_packet_scope(), "unknown family %d", wccp_wccp_address_table.family);
    };

    if (element_tree) {
      proto_item *pi;

      pi = proto_tree_add_string_format_value(element_tree, hf_address_table_element, tvb,
                                              offset, address_length, addr,
                                              "%d: %s", i+1, addr);
      if ((address_length * i) > wccp_wccp_address_table.table_length)
        expert_add_info_format(pinfo, pi, &ei_wccp_length_bad, "Ran out of space to store address");
    }
    EAT(address_length);
  }

  return length;
}
