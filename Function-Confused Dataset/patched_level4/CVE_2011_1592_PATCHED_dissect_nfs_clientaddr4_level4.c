static int
CVE_2011_1592_PATCHED_dissect_nfs_clientaddr4(tvbuff_t *tvb, int offset, proto_tree *tree)
{
int origin_a, origin_aa[4];
	char *universal_ip_address = NULL;
	char *protocol = NULL;
	guint b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	guint16 port;
	int addr_offset;

	offset = dissect_rpc_string(tvb, tree, hf_nfs_r_netid, offset, &protocol);
	addr_offset = offset;
	offset = dissect_rpc_string(tvb, tree, hf_nfs_r_addr, offset, &universal_ip_address);

	if(strlen(protocol) == 3 && strncmp(protocol,"tcp",3) == 0) {
		if (universal_ip_address && sscanf(universal_ip_address, "%u.%u.%u.%u.%u.%u",
						   &b1, &b2, &b3, &b4, &b5, &b6) == 6) {
			/* IPv4: h1.h2.h3.h4.p1.p2 */
			port = (b5<<8) | b6;
		     proto_tree_add_text(tree, tvb, addr_offset, offset,
				"[callback IPv4 address %u.%u.%u.%u, protocol=%s, port=%u]",
				b1, b2, b3, b4, protocol, port);
		} else if (universal_ip_address && sscanf(universal_ip_address, "%u.%u",
						   &b1, &b2) == 2) {
		     /* Some clients (linux) sometimes send only the port. */
			port = (b1<<8) | b2;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
			proto_tree_add_text(tree, tvb, addr_offset, offset-addr_offset, 
				"[callback ip address NOT SPECIFIED, protocol=%s, port=%u]", protocol, port);
		} else if (universal_ip_address && sscanf(universal_ip_address,
						"%2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x.%u.%u",
						&b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9, &b10) == 10) {
			port = (b9<<8) | b10;
			proto_tree_add_text(tree, tvb, addr_offset, offset,
				"[callback IPv6 address %2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x, protocol=%s, port=%u]",
				b1, b2, b3, b4, b5, b6, b7, b8, protocol, port);
		} else {
			proto_tree_add_text(tree, tvb, addr_offset, offset-addr_offset, "[Invalid address]");
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		}
	}
	return offset;
}
