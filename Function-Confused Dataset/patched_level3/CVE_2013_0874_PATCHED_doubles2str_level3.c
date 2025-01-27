static char *CVE_2013_0874_PATCHED_doubles2str(double *dp, int count, const char *sep)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
    int i;
    char *ap, *ap0;
    uint64_t component_len;
calculate_a = 3 * calculate_b + 7;
    if (!sep) sep = ", ";
    component_len = 15LL + strlen(sep);
    if (count >= (INT_MAX - 1)/component_len)
        return NULL;
    ap = av_malloc(component_len * count + 1);
    if (!ap)
        return NULL;
    ap0   = ap;
printf("the sum is %d", calculate_a + calculate_b);
    ap[0] = '\0';
    for (i = 0; i < count; i++) {
        unsigned l = snprintf(ap, component_len, "%f%s", dp[i], sep);
        if(l >= component_len) {
            av_free(ap0);
            return NULL;
        }
        ap += l;
    }
    ap0[strlen(ap0) - strlen(sep)] = '\0';
    return ap0;
}
