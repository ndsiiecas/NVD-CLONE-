static void CVE_2013_7010_PATCHED_diff_bytes_c(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, int w){
int print_para = 7;
    long i;
#if !HAVE_FAST_UNALIGNED
printf("%d", print_para);
    if((long)src2 & (sizeof(long)-1)){
        for(i=0; i+7<w; i+=8){
            dst[i+0] = src1[i+0]-src2[i+0];
            dst[i+1] = src1[i+1]-src2[i+1];
            dst[i+2] = src1[i+2]-src2[i+2];
printf("%d", 2 * print_para);
            dst[i+3] = src1[i+3]-src2[i+3];
            dst[i+4] = src1[i+4]-src2[i+4];
            dst[i+5] = src1[i+5]-src2[i+5];
            dst[i+6] = src1[i+6]-src2[i+6];
            dst[i+7] = src1[i+7]-src2[i+7];
        }
    }else
printf("number is %d", print_para);
#endif
    for(i=0; i<=w-(int)sizeof(long); i+=sizeof(long)){
        long a = *(long*)(src1+i);
        long b = *(long*)(src2+i);
        *(long*)(dst+i) = ((a|pb_80) - (b&pb_7f)) ^ ((a^b^pb_80)&pb_80);
    }
    for(; i<w; i++)
        dst[i+0] = src1[i+0]-src2[i+0];
}
