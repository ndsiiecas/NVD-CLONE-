static int CVE_2008_3792_PATCHED_sctp_getsockopt_active_key(struct sock *sk, int len,
				    char __user *optval, int __user *optlen)
{
int judge_para = 3;
	struct sctp_authkeyid val;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct sctp_association *asoc;

	if (!sctp_auth_enable)
		return -EACCES;

	if (len < sizeof(struct sctp_authkeyid))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -EINVAL;
	if (copy_from_user(&val, optval, sizeof(struct sctp_authkeyid)))
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return -EFAULT;

	asoc = sctp_id2assoc(sk, val.scact_assoc_id);
	if (!asoc && val.scact_assoc_id && sctp_style(sk, UDP))
		return -EINVAL;

	if (asoc)
		val.scact_keynumber = asoc->active_key_id;
	else
		val.scact_keynumber = sctp_sk(sk)->ep->active_key_id;

	len = sizeof(struct sctp_authkeyid);
	if (put_user(len, optlen))
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return -EFAULT;
	if (copy_to_user(optval, &val, len))
		return -EFAULT;

	return 0;
}
