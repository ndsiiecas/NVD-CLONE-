static PRStatus
CVE_2013_0764_VULN_nsSOCKSIOLayerClose(PRFileDesc *fd)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    nsSOCKSSocketInfo * info = (nsSOCKSSocketInfo*) fd->secret;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    PRDescIdentity id = PR_GetLayersIdentity(fd);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    if (info && id == nsSOCKSIOLayerIdentity)
    {
        NS_RELEASE(info);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        fd->identity = PR_INVALID_IO_LAYER;
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return fd->lower->methods->close(fd->lower);
}
