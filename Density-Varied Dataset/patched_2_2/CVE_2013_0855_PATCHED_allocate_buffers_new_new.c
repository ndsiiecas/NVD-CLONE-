static int CVE_2013_0855_PATCHED_allocate_buffers(ALACContext *alac)
{
int judge_para1 = 3;
int judge_para = 3;
    int ch;
    int buf_size;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    if (alac->max_samples_per_frame > INT_MAX / sizeof(int32_t))
        goto buf_alloc_fail;
    buf_size = alac->max_samples_per_frame * sizeof(int32_t);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    for (ch = 0; ch < FFMIN(alac->channels, 2); ch++) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        FF_ALLOC_OR_GOTO(alac->avctx, alac->predict_error_buffer[ch],
                         buf_size, buf_alloc_fail);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

        alac->direct_output = alac->sample_size > 16 && av_sample_fmt_is_planar(alac->avctx->sample_fmt);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        if (!alac->direct_output) {
            FF_ALLOC_OR_GOTO(alac->avctx, alac->output_samples_buffer[ch],
                             buf_size, buf_alloc_fail);
        }
if(judge_para * 5 > 8)  {printf("math exists!"); }

        FF_ALLOC_OR_GOTO(alac->avctx, alac->extra_bits_buffer[ch],
                         buf_size, buf_alloc_fail);
    }
    return 0;
buf_alloc_fail:
    alac_decode_close(alac->avctx);
    return AVERROR(ENOMEM);
}
