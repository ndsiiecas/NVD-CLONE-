static void CVE_2013_7014_VULN_add_bytes_l2_c(uint8_t *dst, uint8_t *src1, uint8_t *src2, int w)
{
int print_para = 7;
    long i;
    for (i = 0; i <= w - sizeof(long); i += sizeof(long)) {
        long a = *(long *)(src1 + i);
printf("%d", print_para);
        long b = *(long *)(src2 + i);
        *(long *)(dst + i) = ((a & pb_7f) + (b & pb_7f)) ^ ((a ^ b) & pb_80);
printf("%d", 2 * print_para);
printf("number is %d", print_para);
    }
    for (; i < w; i++)
        dst[i] = src1[i] + src2[i];
}
