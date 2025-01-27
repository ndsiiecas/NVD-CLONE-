static int CVE_2014_5272_PATCHED_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    IffContext *s = avctx->priv_data;
    const uint8_t *buf = avpkt->size >= 2 ? avpkt->data + AV_RB16(avpkt->data) : NULL;
    const int buf_size = avpkt->size >= 2 ? avpkt->size - AV_RB16(avpkt->data) : 0;
    const uint8_t *buf_end = buf+buf_size;
    int y, plane, res;
    GetByteContext gb;

    if ((res = extract_header(avctx, avpkt)) < 0)
        return res;
    if (s->init) {
        if ((res = avctx->reget_buffer(avctx, &s->frame)) < 0) {
            av_log(avctx, AV_LOG_ERROR, "reget_buffer() failed\n");
            return res;
        }
    } else if ((res = ff_get_buffer(avctx, &s->frame)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return res;
    } else if (avctx->bits_per_coded_sample <= 8 && avctx->pix_fmt == AV_PIX_FMT_PAL8) {
        if ((res = cmap_read_palette(avctx, (uint32_t*)s->frame.data[1])) < 0)
            return res;
    } else if (avctx->pix_fmt == AV_PIX_FMT_RGB32 && avctx->bits_per_coded_sample <= 8) {
        if ((res = cmap_read_palette(avctx, s->mask_palbuf)) < 0)
            return res;
    }
    s->init = 1;

    switch (s->compression) {
    case 0:
        if (avctx->codec_tag == MKTAG('A','C','B','M')) {
            if (avctx->pix_fmt == AV_PIX_FMT_PAL8 || avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                memset(s->frame.data[0], 0, avctx->height * s->frame.linesize[0]);
                for (plane = 0; plane < s->bpp; plane++) {
                    for(y = 0; y < avctx->height && buf < buf_end; y++ ) {
                        uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];
                        decodeplane8(row, buf, FFMIN(s->planesize, buf_end - buf), plane);
                        buf += s->planesize;
                    }
                }
            } else if (s->ham) { // HAM to AV_PIX_FMT_BGR32
                memset(s->frame.data[0], 0, avctx->height * s->frame.linesize[0]);
                for(y = 0; y < avctx->height; y++) {
                    uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];
                    memset(s->ham_buf, 0, s->planesize * 8);
                    for (plane = 0; plane < s->bpp; plane++) {
                        const uint8_t * start = buf + (plane * avctx->height + y) * s->planesize;
                        if (start >= buf_end)
                            break;
                        decodeplane8(s->ham_buf, start, FFMIN(s->planesize, buf_end - start), plane);
                    }
                    decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);
                }
            } else
                return unsupported(avctx);
        } else if (avctx->codec_tag == MKTAG('D','E','E','P')) {
            const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(avctx->pix_fmt);
            int raw_width = avctx->width * (av_get_bits_per_pixel(desc) >> 3);
            int x;
            for(y = 0; y < avctx->height && buf < buf_end; y++ ) {
                uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];
                memcpy(row, buf, FFMIN(raw_width, buf_end - buf));
                buf += raw_width;
                if (avctx->pix_fmt == AV_PIX_FMT_BGR32) {
                    for(x = 0; x < avctx->width; x++)
                        row[4 * x + 3] = row[4 * x + 3] & 0xF0 | (row[4 * x + 3] >> 4);
                }
            }
        } else if (avctx->codec_tag == MKTAG('I','L','B','M')) { // interleaved
            if (avctx->pix_fmt == AV_PIX_FMT_PAL8 || avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                for(y = 0; y < avctx->height; y++ ) {
                    uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];
                    memset(row, 0, avctx->width);
                    for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {
                        decodeplane8(row, buf, FFMIN(s->planesize, buf_end - buf), plane);
                        buf += s->planesize;
                    }
                }
            } else if (s->ham) { // HAM to AV_PIX_FMT_BGR32
                for (y = 0; y < avctx->height; y++) {
                    uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];
                    memset(s->ham_buf, 0, s->planesize * 8);
                    for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {
                        decodeplane8(s->ham_buf, buf, FFMIN(s->planesize, buf_end - buf), plane);
                        buf += s->planesize;
                    }
                    decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);
                }
            } else { // AV_PIX_FMT_BGR32
                for(y = 0; y < avctx->height; y++ ) {
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
                    memset(row, 0, avctx->width << 2);
                    for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {
                        decodeplane32((uint32_t *) row, buf, FFMIN(s->planesize, buf_end - buf), plane);
                        buf += s->planesize;
                    }
                }
            }
        } else if (avctx->codec_tag == MKTAG('P','B','M',' ')) { // IFF-PBM
            if (avctx->pix_fmt == AV_PIX_FMT_PAL8 || avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                for(y = 0; y < avctx->height && buf_end > buf; y++ ) {
                    uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];
                    memcpy(row, buf, FFMIN(avctx->width, buf_end - buf));
                    buf += avctx->width + (avctx->width % 2); // padding if odd
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                }
            } else if (s->ham) { // IFF-PBM: HAM to AV_PIX_FMT_BGR32
                for (y = 0; y < avctx->height && buf_end > buf; y++) {
                    uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];
                    memcpy(s->ham_buf, buf, FFMIN(avctx->width, buf_end - buf));
                    buf += avctx->width + (avctx->width & 1); // padding if odd
                    decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);
                }
            } else
                return unsupported(avctx);
        }
        break;
    case 1:
        if (avctx->codec_tag == MKTAG('I','L','B','M')) { //interleaved
            if (avctx->pix_fmt == AV_PIX_FMT_PAL8 || avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                for(y = 0; y < avctx->height ; y++ ) {
                    uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];
                    memset(row, 0, avctx->width);
                    for (plane = 0; plane < s->bpp; plane++) {
                        buf += decode_byterun(s->planebuf, s->planesize, buf, buf_end);
                        decodeplane8(row, s->planebuf, s->planesize, plane);
                    }
                }
            } else if (avctx->bits_per_coded_sample <= 8) { //8-bit (+ mask) to AV_PIX_FMT_BGR32
                for (y = 0; y < avctx->height ; y++ ) {
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                    memset(s->mask_buf, 0, avctx->width * sizeof(uint32_t));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                    for (plane = 0; plane < s->bpp; plane++) {
                        buf += decode_byterun(s->planebuf, s->planesize, buf, buf_end);
                        decodeplane32(s->mask_buf, s->planebuf, s->planesize, plane);
                    }
                    lookup_pal_indicies((uint32_t *) row, s->mask_buf, s->mask_palbuf, avctx->width);
                }
            } else if (s->ham) { // HAM to AV_PIX_FMT_BGR32
                for (y = 0; y < avctx->height ; y++) {
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
                    memset(s->ham_buf, 0, s->planesize * 8);
                    for (plane = 0; plane < s->bpp; plane++) {
                        buf += decode_byterun(s->planebuf, s->planesize, buf, buf_end);
                        decodeplane8(s->ham_buf, s->planebuf, s->planesize, plane);
                    }
                    decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);
                }
            } else { //AV_PIX_FMT_BGR32
                for(y = 0; y < avctx->height ; y++ ) {
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
                    memset(row, 0, avctx->width << 2);
                    for (plane = 0; plane < s->bpp; plane++) {
                        buf += decode_byterun(s->planebuf, s->planesize, buf, buf_end);
                        decodeplane32((uint32_t *) row, s->planebuf, s->planesize, plane);
                    }
                }
            }
        } else if (avctx->codec_tag == MKTAG('P','B','M',' ')) { // IFF-PBM
            if (avctx->pix_fmt == AV_PIX_FMT_PAL8 || avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                for(y = 0; y < avctx->height ; y++ ) {
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
                    buf += decode_byterun(row, avctx->width, buf, buf_end);
                }
            } else if (s->ham) { // IFF-PBM: HAM to AV_PIX_FMT_BGR32
                for (y = 0; y < avctx->height ; y++) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                    uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];
                    buf += decode_byterun(s->ham_buf, avctx->width, buf, buf_end);
                    decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);
                }
            } else
                return unsupported(avctx);
        } else if (avctx->codec_tag == MKTAG('D','E','E','P')) { // IFF-DEEP
            const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(avctx->pix_fmt);
            if (av_get_bits_per_pixel(desc) == 32)
                decode_deep_rle32(s->frame.data[0], buf, buf_size, avctx->width, avctx->height, s->frame.linesize[0]);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            else
                return unsupported(avctx);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        }
        break;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    case 4:
        bytestream2_init(&gb, buf, buf_size);
        if (avctx->codec_tag == MKTAG('R','G','B','8') && avctx->pix_fmt == AV_PIX_FMT_RGB32)
            decode_rgb8(&gb, s->frame.data[0], avctx->width, avctx->height, s->frame.linesize[0]);
        else if (avctx->codec_tag == MKTAG('R','G','B','N') && avctx->pix_fmt == AV_PIX_FMT_RGB444)
if(judge_para * 5 > 8)  {printf("math exists!"); }
            decode_rgbn(&gb, s->frame.data[0], avctx->width, avctx->height, s->frame.linesize[0]);
        else
            return unsupported(avctx);
        break;
    case 5:
        if (avctx->codec_tag == MKTAG('D','E','E','P')) {
            const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(avctx->pix_fmt);
            if (av_get_bits_per_pixel(desc) == 32)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                decode_deep_tvdc32(s->frame.data[0], buf, buf_size, avctx->width, avctx->height, s->frame.linesize[0], s->tvdc);
            else
                return unsupported(avctx);
        } else
            return unsupported(avctx);
        break;
    default:
        return unsupported(avctx);
    }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

    *got_frame = 1;
    *(AVFrame*)data = s->frame;
    return buf_size;
}
