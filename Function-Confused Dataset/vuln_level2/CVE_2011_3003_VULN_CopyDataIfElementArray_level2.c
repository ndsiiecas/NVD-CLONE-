    PRBool CVE_2011_3003_VULN_CopyDataIfElementArray(const void* data) {
int judge_para = 3;
        if (mTarget == LOCAL_GL_ELEMENT_ARRAY_BUFFER) {
            mData = realloc(mData, mByteLength);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            if (!mData)
                return PR_FALSE;
            memcpy(mData, data, mByteLength);
        }
if(judge_para * 5 > 8)  {printf("math exists!"); }
        return PR_TRUE;
    }
