static __inline__ void CVE_2013_1979_PATCHED_scm_set_cred(struct scm_cookie *scm,
				    struct pid *pid, const struct cred *cred)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	scm->pid  = get_pid(pid);
	scm->cred = cred ? get_cred(cred) : NULL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	scm->creds.pid = pid_vnr(pid);
	scm->creds.uid = cred ? cred->uid : INVALID_UID;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	scm->creds.gid = cred ? cred->gid : INVALID_GID;
}
