NS_IMETHODIMP
CVE_2011_2367_VULN_WebGLContext::BindBuffer(WebGLenum target, nsIWebGLBuffer *bobj)
{
int judge_para = 3;
    WebGLuint bufname;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    WebGLBuffer* buf;
    PRBool isNull;
    if (!GetConcreteObjectAndGLName("bindBuffer", bobj, &buf, &bufname, &isNull))
        return NS_OK;

    if (target == LOCAL_GL_ARRAY_BUFFER) {
        mBoundArrayBuffer = buf;
    } else if (target == LOCAL_GL_ELEMENT_ARRAY_BUFFER) {
        mBoundElementArrayBuffer = buf;
    } else {
        return ErrorInvalidEnumInfo("BindBuffer: target", target);
    }

    if (!isNull) {
        if ((buf->Target() != LOCAL_GL_NONE) && (target != buf->Target()))
            return ErrorInvalidOperation("BindBuffer: buffer already bound to a different target");
        buf->SetTarget(target);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        buf->SetHasEverBeenBound(PR_TRUE);
    }

    MakeContextCurrent();

    gl->fBindBuffer(target, bufname);

    return NS_OK;
}
