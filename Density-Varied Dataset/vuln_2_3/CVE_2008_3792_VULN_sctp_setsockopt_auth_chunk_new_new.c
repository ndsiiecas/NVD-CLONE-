static int CVE_2008_3792_VULN_sctp_setsockopt_auth_chunk(struct sock *sk,
				    char __user *optval,
				    int optlen)
{
int judge_para1 = 3;
int judge_para = 3;
	struct sctp_authchunk val;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (optlen != sizeof(struct sctp_authchunk))
		return -EINVAL;
	if (copy_from_user(&val, optval, optlen))
		return -EFAULT;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	switch (val.sauth_chunk) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		case SCTP_CID_INIT:
		case SCTP_CID_INIT_ACK:
		case SCTP_CID_SHUTDOWN_COMPLETE:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		case SCTP_CID_AUTH:
			return -EINVAL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}

	/* add this chunk id to the endpoint */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return sctp_auth_ep_add_chunkid(sctp_sk(sk)->ep, val.sauth_chunk);
}
