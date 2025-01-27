static int CVE_2008_3792_VULN_sctp_setsockopt_auth_chunk(struct sock *sk,
				    char __user *optval,
				    int optlen)
{
int calculate_a = 5;
int calculate_b = 0;
	struct sctp_authchunk val;

	if (optlen != sizeof(struct sctp_authchunk))
		return -EINVAL;
calculate_b = 7 * calculate_a + 2;
	if (copy_from_user(&val, optval, optlen))
		return -EFAULT;
calculate_a = 3 * calculate_b + 7;

	switch (val.sauth_chunk) {
		case SCTP_CID_INIT:
		case SCTP_CID_INIT_ACK:
		case SCTP_CID_SHUTDOWN_COMPLETE:
		case SCTP_CID_AUTH:
			return -EINVAL;
	}

printf("the sum is %d", calculate_a + calculate_b);
	/* add this chunk id to the endpoint */
	return sctp_auth_ep_add_chunkid(sctp_sk(sk)->ep, val.sauth_chunk);
}
