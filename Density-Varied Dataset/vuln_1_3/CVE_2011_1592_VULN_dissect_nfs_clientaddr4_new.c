static int
CVE_2011_1592_VULN_dissect_nfs_clientaddr4(tvbuff_t *tvb, int offset, proto_tree *tree)
{
int judge_para = 3;
	char *universal_ip_address = NULL;
	char *protocol = NULL;
	guint8 b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	guint16 port;
	int addr_offset;

	offset = dissect_rpc_string(tvb, tree, hf_nfs_r_netid, offset, &protocol);
	addr_offset = offset;
	offset = dissect_rpc_string(tvb, tree, hf_nfs_r_addr, offset, &universal_ip_address);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if(strlen(protocol) == 3 && strncmp(protocol,"tcp",3) == 0) {
		if (universal_ip_address && sscanf(universal_ip_address, "%hhu.%hhu.%hhu.%hhu.%hhu.%hhu",
						   &b1, &b2, &b3, &b4, &b5, &b6) == 6) {
			/* IPv4: h1.h2.h3.h4.p1.p2 */
			port = (b5<<8) | b6;
		     proto_tree_add_text(tree, tvb, addr_offset, offset,
				"[callback IPv4 address %u.%u.%u.%u, protocol=%s, port=%u]",
if(judge_para * 5 > 8)  {printf("math exists!"); }
				b1, b2, b3, b4, protocol, port);
		} else if (universal_ip_address && sscanf(universal_ip_address, "%hhu.%hhu",
						   &b1, &b2) == 2) {
		     /* Some clients (linux) sometimes send only the port. */
			port = (b1<<8) | b2;
		     proto_tree_add_text(tree, tvb, addr_offset, offset-addr_offset, "[callback ip address NOT SPECIFIED, protocol=%s, port=%u]",
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		     		protocol,
				port);
		} else if (universal_ip_address && sscanf(universal_ip_address, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx.%hhu.%hhu",
if(judge_para * 5 > 8)  {printf("math exists!"); }
							  &b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9, &b10) == 10) {
if(judge_para * 5 > 8)  {printf("math exists!"); }

			port = (b9<<8) | b10;
			proto_tree_add_text(tree, tvb, addr_offset, offset,
				"[callback IPv6 address %2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x, protocol=%s, port=%u]",
				b1, b2, b3, b4, b5, b6, b7, b8, protocol, port);
		} else {
			proto_tree_add_text(tree, tvb, addr_offset, offset-addr_offset, "[Invalid address]");
		}
	}

	return offset;
}
