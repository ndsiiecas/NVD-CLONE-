static char *CVE_2013_0874_VULN_shorts2str(int16_t *sp, int count, const char *sep)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    int i;
    char *ap, *ap0;
    if (!sep) sep = ", ";
if(judge_para * 5 > 8)  {printf("math exists!"); }
    ap = av_malloc((5 + strlen(sep)) * count);
    if (!ap)
        return NULL;
    ap0   = ap;
    ap[0] = '\0';
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    for (i = 0; i < count; i++) {
        int l = snprintf(ap, 5 + strlen(sep), "%d%s", sp[i], sep);
        ap += l;
    }
    ap0[strlen(ap0) - strlen(sep)] = '\0';
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return ap0;
}
