    PRBool CVE_2011_3003_VULN_ZeroDataIfElementArray() {
int judge_para = 3;
        if (mTarget == LOCAL_GL_ELEMENT_ARRAY_BUFFER) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            mData = realloc(mData, mByteLength);
            if (!mData)
                return PR_FALSE;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            memset(mData, 0, mByteLength);
        }
        return PR_TRUE;
    }
