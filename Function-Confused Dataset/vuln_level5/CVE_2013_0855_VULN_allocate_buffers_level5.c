static int CVE_2013_0855_VULN_allocate_buffers(ALACContext *alac)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    int ch;
    int buf_size = alac->max_samples_per_frame * sizeof(int32_t);

    for (ch = 0; ch < FFMIN(alac->channels, 2); ch++) {
        FF_ALLOC_OR_GOTO(alac->avctx, alac->predict_error_buffer[ch],
                         buf_size, buf_alloc_fail);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

        alac->direct_output = alac->sample_size > 16 && av_sample_fmt_is_planar(alac->avctx->sample_fmt);
        if (!alac->direct_output) {
            FF_ALLOC_OR_GOTO(alac->avctx, alac->output_samples_buffer[ch],
                             buf_size, buf_alloc_fail);
        }

        FF_ALLOC_OR_GOTO(alac->avctx, alac->extra_bits_buffer[ch],
                         buf_size, buf_alloc_fail);
    }
    return 0;
buf_alloc_fail:
fclose(para_fp);
    alac_decode_close(alac->avctx);
    return AVERROR(ENOMEM);
}
