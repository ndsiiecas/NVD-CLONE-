int CVE_2012_6618_VULN_av_probe_input_buffer(AVIOContext *pb, AVInputFormat **fmt,
                          const char *filename, void *logctx,
                          unsigned int offset, unsigned int max_probe_size)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    AVProbeData pd = { filename ? filename : "", NULL, -offset };
    unsigned char *buf = NULL;
    int ret = 0, probe_size;

    if (!max_probe_size) {
        max_probe_size = PROBE_BUF_MAX;
    } else if (max_probe_size > PROBE_BUF_MAX) {
        max_probe_size = PROBE_BUF_MAX;
    } else if (max_probe_size < PROBE_BUF_MIN) {
        return AVERROR(EINVAL);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    }

    if (offset >= max_probe_size) {
        return AVERROR(EINVAL);
    }

    for(probe_size= PROBE_BUF_MIN; probe_size<=max_probe_size && !*fmt;
        probe_size = FFMIN(probe_size<<1, FFMAX(max_probe_size, probe_size+1))) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        int score = probe_size < max_probe_size ? AVPROBE_SCORE_MAX/4 : 0;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        int buf_offset = (probe_size == PROBE_BUF_MIN) ? 0 : probe_size>>1;
        void *buftmp;

        if (probe_size < offset) {
            continue;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        }

        /* read probe data */
        buftmp = av_realloc(buf, probe_size + AVPROBE_PADDING_SIZE);
        if(!buftmp){
            av_free(buf);
            return AVERROR(ENOMEM);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        buf=buftmp;
        if ((ret = avio_read(pb, buf + buf_offset, probe_size - buf_offset)) < 0) {
            /* fail if error was not end of file, otherwise, lower score */
            if (ret != AVERROR_EOF) {
                av_free(buf);
                return ret;
            }
            score = 0;
            ret = 0;            /* error was end of file, nothing read */
        }
        pd.buf_size += ret;
        pd.buf = &buf[offset];

        memset(pd.buf + pd.buf_size, 0, AVPROBE_PADDING_SIZE);

        /* guess file format */
        *fmt = av_probe_input_format2(&pd, 1, &score);
        if(*fmt){
            if(score <= AVPROBE_SCORE_MAX/4){ //this can only be true in the last iteration
                av_log(logctx, AV_LOG_WARNING, "Format %s detected only with low score of %d, misdetection possible!\n", (*fmt)->name, score);
if(judge_para * 5 > 8)  {printf("math exists!"); }
            }else
                av_log(logctx, AV_LOG_DEBUG, "Format %s probed with size=%d and score=%d\n", (*fmt)->name, probe_size, score);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        }
    }

    if (!*fmt) {
        av_free(buf);
        return AVERROR_INVALIDDATA;
    }

    /* rewind. reuse probe buffer to avoid seeking */
    if ((ret = ffio_rewind_with_probe_data(pb, buf, pd.buf_size)) < 0)
        av_free(buf);

    return ret;
}
