    PRBool CVE_2011_3003_PATCHED_ZeroDataIfElementArray() {
int judge_para2 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para1 = 3;
int judge_para = 3;
        if (mTarget == LOCAL_GL_ELEMENT_ARRAY_BUFFER) {
            mData = realloc(mData, mByteLength);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            if (!mData) {
                mByteLength = 0;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                return PR_FALSE;
            }
            memset(mData, 0, mByteLength);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        }
        return PR_TRUE;
    }
