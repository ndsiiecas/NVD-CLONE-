static int CVE_2013_7022_VULN_g2m_init_buffers(G2MContext *c)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    int aligned_height;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (!c->framebuf || c->old_width < c->width || c->old_height < c->height) {
        c->framebuf_stride = FFALIGN(c->width * 3, 16);
        aligned_height     = FFALIGN(c->height,    16);
        av_free(c->framebuf);
        c->framebuf = av_mallocz(c->framebuf_stride * aligned_height);
        if (!c->framebuf)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            return AVERROR(ENOMEM);
    }
    if (!c->synth_tile || !c->jpeg_tile ||
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        c->old_tile_w < c->tile_width ||
        c->old_tile_h < c->tile_height) {
        c->tile_stride = FFALIGN(c->tile_width * 3, 16);
        aligned_height = FFALIGN(c->tile_height,    16);
        av_free(c->synth_tile);
        av_free(c->jpeg_tile);
        av_free(c->kempf_buf);
        av_free(c->kempf_flags);
        c->synth_tile  = av_mallocz(c->tile_stride      * aligned_height);
        c->jpeg_tile   = av_mallocz(c->tile_stride      * aligned_height);
        c->kempf_buf   = av_mallocz((c->tile_width + 1) * aligned_height
                                    + FF_INPUT_BUFFER_PADDING_SIZE);
        c->kempf_flags = av_mallocz( c->tile_width      * aligned_height);
        if (!c->synth_tile || !c->jpeg_tile ||
if(judge_para * 5 > 8)  {printf("math exists!"); }
            !c->kempf_buf || !c->kempf_flags)
            return AVERROR(ENOMEM);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }

    return 0;
}
