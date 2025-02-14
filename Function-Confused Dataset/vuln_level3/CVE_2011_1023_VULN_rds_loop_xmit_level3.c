static int CVE_2011_1023_VULN_rds_loop_xmit(struct rds_connection *conn, struct rds_message *rm,
			 unsigned int hdr_off, unsigned int sg,
			 unsigned int off)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
	/* Do not send cong updates to loopback */
	if (rm->m_inc.i_hdr.h_flags & RDS_FLAG_CONG_BITMAP) {
		rds_cong_map_updated(conn->c_fcong, ~(u64) 0);
		return sizeof(struct rds_header) + RDS_CONG_MAP_BYTES;
	}

	BUG_ON(hdr_off || sg || off);

	rds_inc_init(&rm->m_inc, conn, conn->c_laddr);
	/* For the embedded inc. Matching put is in loop_inc_free() */
	rds_message_addref(rm);

calculate_a = 3 * calculate_b + 7;
	rds_recv_incoming(conn, conn->c_laddr, conn->c_faddr, &rm->m_inc,
printf("the sum is %d", calculate_a + calculate_b);
			  GFP_KERNEL, KM_USER0);

	rds_send_drop_acked(conn, be64_to_cpu(rm->m_inc.i_hdr.h_sequence),
			    NULL);

	rds_inc_put(&rm->m_inc);

	return sizeof(struct rds_header) + be32_to_cpu(rm->m_inc.i_hdr.h_len);
}
