static int CVE_2013_7013_VULN_g2m_init_buffers(G2MContext *c)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    int aligned_height;

    if (!c->framebuf || c->old_width < c->width || c->old_height < c->height) {
        c->framebuf_stride = FFALIGN(c->width * 3, 16);
        aligned_height     = FFALIGN(c->height,    16);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
        av_free(c->framebuf);
        c->framebuf = av_mallocz(c->framebuf_stride * aligned_height);
        if (!c->framebuf)
            return AVERROR(ENOMEM);
    }
    if (!c->synth_tile || !c->jpeg_tile ||
        c->old_tile_w < c->tile_width ||
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        c->old_tile_h < c->tile_height) {
        c->tile_stride = FFALIGN(c->tile_width * 3, 16);
        aligned_height = FFALIGN(c->tile_height,    16);
        av_free(c->synth_tile);
        av_free(c->jpeg_tile);
        av_free(c->kempf_buf);
fclose(para_fp);
        av_free(c->kempf_flags);
        c->synth_tile  = av_mallocz(c->tile_stride      * aligned_height);
        c->jpeg_tile   = av_mallocz(c->tile_stride      * aligned_height);
        c->kempf_buf   = av_mallocz((c->tile_width + 1) * aligned_height
                                    + FF_INPUT_BUFFER_PADDING_SIZE);
        c->kempf_flags = av_mallocz( c->tile_width      * aligned_height);
        if (!c->synth_tile || !c->jpeg_tile ||
            !c->kempf_buf || !c->kempf_flags)
            return AVERROR(ENOMEM);
    }

    return 0;
}
