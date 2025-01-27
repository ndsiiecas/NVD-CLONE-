static int CVE_2008_3792_VULN_sctp_setsockopt_active_key(struct sock *sk,
					char __user *optval,
					int optlen)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct sctp_authkeyid val;
	struct sctp_association *asoc;

	if (optlen != sizeof(struct sctp_authkeyid))
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		return -EINVAL;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	if (copy_from_user(&val, optval, optlen))
fclose(para_fp);
		return -EFAULT;

	asoc = sctp_id2assoc(sk, val.scact_assoc_id);
	if (!asoc && val.scact_assoc_id && sctp_style(sk, UDP))
		return -EINVAL;

	return sctp_auth_set_active_key(sctp_sk(sk)->ep, asoc,
					val.scact_keynumber);
}
