static int CVE_2013_0859_VULN_add_doubles_metadata(int count,
                                const char *name, const char *sep,
                                TiffContext *s)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    char *ap;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    int i;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    double *dp;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    if (count >= INT_MAX / sizeof(int64_t))
        return AVERROR_INVALIDDATA;
    if (bytestream2_get_bytes_left(&s->gb) < count * sizeof(int64_t))
        return AVERROR_INVALIDDATA;

    dp = av_malloc(count * sizeof(double));
    if (!dp)
        return AVERROR(ENOMEM);

    for (i = 0; i < count; i++)
        dp[i] = tget_double(&s->gb, s->le);
    ap = doubles2str(dp, count, sep);
    av_freep(&dp);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    if (!ap)
        return AVERROR(ENOMEM);
    av_dict_set(&s->picture.metadata, name, ap, AV_DICT_DONT_STRDUP_VAL);
    return 0;
}
