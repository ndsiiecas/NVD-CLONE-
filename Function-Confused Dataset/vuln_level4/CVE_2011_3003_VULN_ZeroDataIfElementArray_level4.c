    PRBool CVE_2011_3003_VULN_ZeroDataIfElementArray() {
int origin_a, origin_aa[4];
        if (mTarget == LOCAL_GL_ELEMENT_ARRAY_BUFFER) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
            mData = realloc(mData, mByteLength);
            if (!mData)
                return PR_FALSE;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
            memset(mData, 0, mByteLength);
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
        }
        return PR_TRUE;
    }
