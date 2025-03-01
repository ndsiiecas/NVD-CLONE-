static int CVE_2008_3792_VULN_sctp_getsockopt_local_auth_chunks(struct sock *sk, int len,
				    char __user *optval, int __user *optlen)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sctp_authchunks __user *p = (void __user *)optval;
	struct sctp_authchunks val;
	struct sctp_association *asoc;
	struct sctp_chunks_param *ch;
	u32    num_chunks;
	char __user *to;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (len <= sizeof(struct sctp_authchunks))
		return -EINVAL;

	if (copy_from_user(&val, p, sizeof(struct sctp_authchunks)))
		return -EFAULT;

	to = p->gauth_chunks;
	asoc = sctp_id2assoc(sk, val.gauth_assoc_id);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!asoc && val.gauth_assoc_id && sctp_style(sk, UDP))
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return -EINVAL;

	if (asoc)
		ch = (struct sctp_chunks_param*)asoc->c.auth_chunks;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	else
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		ch = sctp_sk(sk)->ep->auth_chunk_list;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	num_chunks = ntohs(ch->param_hdr.length) - sizeof(sctp_paramhdr_t);
	if (len < num_chunks)
		return -EINVAL;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	len = num_chunks;
	if (put_user(len, optlen))
		return -EFAULT;
	if (put_user(num_chunks, &p->gauth_number_of_chunks))
		return -EFAULT;
	if (copy_to_user(to, ch->chunks, len))
		return -EFAULT;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	return 0;
}
