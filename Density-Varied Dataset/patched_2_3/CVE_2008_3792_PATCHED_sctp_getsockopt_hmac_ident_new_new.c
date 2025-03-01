static int CVE_2008_3792_PATCHED_sctp_getsockopt_hmac_ident(struct sock *sk, int len,
				    char __user *optval, int __user *optlen)
{
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	struct sctp_hmacalgo  __user *p = (void __user *)optval;
	struct sctp_hmac_algo_param *hmacs;
	__u16 data_len = 0;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	u32 num_idents;

	if (!sctp_auth_enable)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return -EACCES;

	hmacs = sctp_sk(sk)->ep->auth_hmacs_list;
	data_len = ntohs(hmacs->param_hdr.length) - sizeof(sctp_paramhdr_t);

	if (len < sizeof(struct sctp_hmacalgo) + data_len)
		return -EINVAL;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	len = sizeof(struct sctp_hmacalgo) + data_len;
	num_idents = data_len / sizeof(u16);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (put_user(len, optlen))
		return -EFAULT;
	if (put_user(num_idents, &p->shmac_num_idents))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -EFAULT;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (copy_to_user(p->shmac_idents, hmacs->hmac_ids, data_len))
		return -EFAULT;
	return 0;
}
