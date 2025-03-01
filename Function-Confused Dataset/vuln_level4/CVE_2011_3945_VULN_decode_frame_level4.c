static int CVE_2011_3945_VULN_decode_frame(AVCodecContext *avctx, void *data, int *data_size, AVPacket *avpkt)
{
int origin_a, origin_aa[4];
    const uint8_t *buf = avpkt->data;
    const uint8_t *buf_end = buf + avpkt->size;
    KgvContext * const c = avctx->priv_data;
    int offsets[7];
    uint16_t *out, *prev;
    int outcnt = 0, maxcnt;
    int w, h, i;

    if (avpkt->size < 2)
        return -1;

    w = (buf[0] + 1) * 8;
    h = (buf[1] + 1) * 8;
    buf += 2;

    if (av_image_check_size(w, h, 0, avctx))
        return -1;

    if (w != avctx->width || h != avctx->height)
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        avcodec_set_dimensions(avctx, w, h);

    maxcnt = w * h;

    out = av_realloc(c->cur, w * h * 2);
    if (!out)
        return -1;
    c->cur = out;

    prev = av_realloc(c->prev, w * h * 2);
    if (!prev)
        return -1;
    c->prev = prev;

    for (i = 0; i < 7; i++)
        offsets[i] = -1;

    while (outcnt < maxcnt && buf_end - 2 > buf) {
        int code = AV_RL16(buf);
        buf += 2;

        if (!(code & 0x8000)) {
            out[outcnt++] = code; // rgb555 pixel coded directly
        } else {
            int count;
            uint16_t *inp;

            if ((code & 0x6000) == 0x6000) {
                // copy from previous frame
                int oidx = (code >> 10) & 7;
                int start;

                count = (code & 0x3FF) + 3;

                if (offsets[oidx] < 0) {
                    if (buf_end - 3 < buf)
                        break;
                    offsets[oidx] = AV_RL24(buf);
                    buf += 3;
                }

                start = (outcnt + offsets[oidx]) % maxcnt;

                if (maxcnt - start < count)
                    break;

                inp = prev + start;
            } else {
                // copy from earlier in this frame
                int offset = (code & 0x1FFF) + 1;

                if (!(code & 0x6000)) {
                    count = 2;
                } else if ((code & 0x6000) == 0x2000) {
                    count = 3;
                } else {
                    if (buf_end - 1 < buf)
                        break;
                    count = 4 + *buf++;
                }

                if (outcnt < offset)
                    break;

                inp = out + outcnt - offset;
            }

            if (maxcnt - outcnt < count)
                break;

            for (i = 0; i < count; i++)
                out[outcnt++] = inp[i];
        }
    }

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    if (outcnt - maxcnt)
        av_log(avctx, AV_LOG_DEBUG, "frame finished with %d diff\n", outcnt - maxcnt);

    c->pic.data[0]     = (uint8_t *)c->cur;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
    c->pic.linesize[0] = w * 2;

    *data_size = sizeof(AVFrame);
    *(AVFrame*)data = c->pic;

    FFSWAP(uint16_t *, c->cur, c->prev);

    return avpkt->size;
}
