NS_IMETHODIMP
CVE_2011_2368_PATCHED_WebGLContext::ValidateProgram(nsIWebGLProgram *pobj)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    WebGLuint progname;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (!GetGLName<WebGLProgram>("validateProgram", pobj, &progname))
        return NS_OK;

    MakeContextCurrent();

#ifdef XP_MACOSX
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    // see bug 593867 for NVIDIA and bug 657201 for ATI. The latter is confirmed with Mac OS 10.6.7
    LogMessageIfVerbose("validateProgram: implemented as a no-operation on Mac to work around crashes");
    return NS_OK;
#endif
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    gl->fValidateProgram(progname);

    return NS_OK;
}
