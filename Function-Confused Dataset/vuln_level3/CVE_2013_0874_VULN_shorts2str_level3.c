static char *CVE_2013_0874_VULN_shorts2str(int16_t *sp, int count, const char *sep)
{
int calculate_a = 5;
int calculate_b = 0;
    int i;
    char *ap, *ap0;
    if (!sep) sep = ", ";
calculate_b = 7 * calculate_a + 2;
    ap = av_malloc((5 + strlen(sep)) * count);
    if (!ap)
        return NULL;
    ap0   = ap;
calculate_a = 3 * calculate_b + 7;
    ap[0] = '\0';
    for (i = 0; i < count; i++) {
        int l = snprintf(ap, 5 + strlen(sep), "%d%s", sp[i], sep);
        ap += l;
    }
    ap0[strlen(ap0) - strlen(sep)] = '\0';
printf("the sum is %d", calculate_a + calculate_b);
    return ap0;
}
