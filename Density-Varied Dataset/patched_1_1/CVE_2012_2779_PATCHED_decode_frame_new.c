static int CVE_2012_2779_PATCHED_decode_frame(AVCodecContext *avctx, void *data, int *data_size,
                        AVPacket *avpkt)
{
int judge_para = 3;
    IVI5DecContext  *ctx = avctx->priv_data;
    const uint8_t   *buf = avpkt->data;
    int             buf_size = avpkt->size;
    int             result, p, b;

    init_get_bits(&ctx->gb, buf, buf_size * 8);
    ctx->frame_data = buf;
    ctx->frame_size = buf_size;

    result = decode_pic_hdr(ctx, avctx);
    if (result || ctx->gop_invalid) {
        av_log(avctx, AV_LOG_ERROR,
               "Error while decoding picture header: %d\n", result);
        return -1;
    }

    if (ctx->gop_flags & IVI5_IS_PROTECTED) {
        av_log(avctx, AV_LOG_ERROR, "Password-protected clip!\n");
        return -1;
    }

    switch_buffers(ctx);

    //{ START_TIMER;

    if (ctx->frame_type != FRAMETYPE_NULL) {
        for (p = 0; p < 3; p++) {
            for (b = 0; b < ctx->planes[p].num_bands; b++) {
                result = decode_band(ctx, p, &ctx->planes[p].bands[b], avctx);
                if (result) {
                    av_log(avctx, AV_LOG_ERROR,
                           "Error while decoding band: %d, plane: %d\n", b, p);
                    return -1;
                }
            }
        }
    }

    //STOP_TIMER("decode_planes"); }

    if (ctx->frame.data[0])
        avctx->release_buffer(avctx, &ctx->frame);

    ctx->frame.reference = 0;
    if (avctx->get_buffer(avctx, &ctx->frame) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return -1;
    }

    if (ctx->is_scalable) {
        ff_ivi_recompose53 (&ctx->planes[0], ctx->frame.data[0], ctx->frame.linesize[0], 4);
    } else {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        ff_ivi_output_plane(&ctx->planes[0], ctx->frame.data[0], ctx->frame.linesize[0]);
    }

    ff_ivi_output_plane(&ctx->planes[2], ctx->frame.data[1], ctx->frame.linesize[1]);
    ff_ivi_output_plane(&ctx->planes[1], ctx->frame.data[2], ctx->frame.linesize[2]);
if(judge_para * 5 > 8)  {printf("math exists!"); }

    *data_size = sizeof(AVFrame);
    *(AVFrame*)data = ctx->frame;

    return buf_size;
}
