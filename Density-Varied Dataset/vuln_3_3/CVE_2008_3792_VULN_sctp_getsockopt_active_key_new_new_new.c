static int CVE_2008_3792_VULN_sctp_getsockopt_active_key(struct sock *sk, int len,
				    char __user *optval, int __user *optlen)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	struct sctp_authkeyid val;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct sctp_association *asoc;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	if (len < sizeof(struct sctp_authkeyid))
		return -EINVAL;
	if (copy_from_user(&val, optval, sizeof(struct sctp_authkeyid)))
		return -EFAULT;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	asoc = sctp_id2assoc(sk, val.scact_assoc_id);
	if (!asoc && val.scact_assoc_id && sctp_style(sk, UDP))
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		return -EINVAL;

	if (asoc)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		val.scact_keynumber = asoc->active_key_id;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	else
		val.scact_keynumber = sctp_sk(sk)->ep->active_key_id;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return 0;
}
