static void CVE_2013_7014_VULN_add_bytes_l2_c(uint8_t *dst, uint8_t *src1, uint8_t *src2, int w)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    long i;
    for (i = 0; i <= w - sizeof(long); i += sizeof(long)) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        long a = *(long *)(src1 + i);
        long b = *(long *)(src2 + i);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        *(long *)(dst + i) = ((a & pb_7f) + (b & pb_7f)) ^ ((a ^ b) & pb_80);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    }
    for (; i < w; i++)
        dst[i] = src1[i] + src2[i];
}
