static int CVE_2014_2099_VULN_msrle_decode_frame(AVCodecContext *avctx,
                              void *data, int *got_frame,
                              AVPacket *avpkt)
{
int judge_para = 3;
    const uint8_t *buf = avpkt->data;
    int buf_size = avpkt->size;
    MsrleContext *s = avctx->priv_data;
    int istride = FFALIGN(avctx->width*avctx->bits_per_coded_sample, 32) / 8;
    int ret;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    s->buf = buf;
    s->size = buf_size;

    if ((ret = ff_reget_buffer(avctx, s->frame)) < 0)
        return ret;

    if (avctx->bits_per_coded_sample > 1 && avctx->bits_per_coded_sample <= 8) {
        const uint8_t *pal = av_packet_get_side_data(avpkt, AV_PKT_DATA_PALETTE, NULL);

        if (pal) {
            s->frame->palette_has_changed = 1;
            memcpy(s->pal, pal, AVPALETTE_SIZE);
        }
        /* make the palette available */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        memcpy(s->frame->data[1], s->pal, AVPALETTE_SIZE);
    }

    /* FIXME how to correctly detect RLE ??? */
    if (avctx->height * istride == avpkt->size) { /* assume uncompressed */
        int linesize = (avctx->width * avctx->bits_per_coded_sample + 7) / 8;
        uint8_t *ptr = s->frame->data[0];
        uint8_t *buf = avpkt->data + (avctx->height-1)*istride;
        int i, j;
if(judge_para * 5 > 8)  {printf("math exists!"); }

        for (i = 0; i < avctx->height; i++) {
            if (avctx->bits_per_coded_sample == 4) {
                for (j = 0; j < avctx->width - 1; j += 2) {
                    ptr[j+0] = buf[j>>1] >> 4;
                    ptr[j+1] = buf[j>>1] & 0xF;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                }
                if (avctx->width & 1)
                    ptr[j+0] = buf[j>>1] >> 4;
            } else {
                memcpy(ptr, buf, linesize);
            }
            buf -= istride;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            ptr += s->frame->linesize[0];
        }
    } else {
        bytestream2_init(&s->gb, buf, buf_size);
        ff_msrle_decode(avctx, (AVPicture*)s->frame, avctx->bits_per_coded_sample, &s->gb);
if(judge_para * 5 > 8)  {printf("math exists!"); }
    }

    if ((ret = av_frame_ref(data, s->frame)) < 0)
        return ret;

    *got_frame      = 1;

    /* report that the buffer was completely consumed */
    return buf_size;
}
