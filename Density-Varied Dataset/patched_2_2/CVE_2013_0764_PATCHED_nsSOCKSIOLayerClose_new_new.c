static PRStatus
CVE_2013_0764_PATCHED_nsSOCKSIOLayerClose(PRFileDesc *fd)
{
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
    nsSOCKSSocketInfo * info = (nsSOCKSSocketInfo*) fd->secret;
    PRDescIdentity id = PR_GetLayersIdentity(fd);

    if (info && id == nsSOCKSIOLayerIdentity)
    {
        info->ForgetFD();
        NS_RELEASE(info);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        fd->identity = PR_INVALID_IO_LAYER;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return fd->lower->methods->close(fd->lower);
}
