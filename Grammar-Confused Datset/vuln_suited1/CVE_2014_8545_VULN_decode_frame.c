static int CVE_2014_8545_VULN_decode_frame(AVCodecContext *avctx,
                        void *data, int *got_frame,
                        AVPacket *avpkt)
{
    PNGDecContext *const s = avctx->priv_data;
    const uint8_t *buf     = avpkt->data;
    int buf_size           = avpkt->size;
    AVFrame *p;
    AVDictionary *metadata  = NULL;
    uint32_t tag, length;
    int64_t sig;
    int ret;

    ff_thread_release_buffer(avctx, &s->last_picture);
    FFSWAP(ThreadFrame, s->picture, s->last_picture);
    p = s->picture.f;

    bytestream2_init(&s->gb, buf, buf_size);

    /* check signature */
    sig = bytestream2_get_be64(&s->gb);
    if (sig != PNGSIG &&
        sig != MNGSIG) {
        av_log(avctx, AV_LOG_ERROR, "Missing png signature\n");
        return AVERROR_INVALIDDATA;
    }

    s->y = s->state = 0;

    /* init the zlib */
    s->zstream.zalloc = ff_png_zalloc;
    s->zstream.zfree  = ff_png_zfree;
    s->zstream.opaque = NULL;
    ret = inflateInit(&s->zstream);
    if (ret != Z_OK) {
        av_log(avctx, AV_LOG_ERROR, "inflateInit returned error %d\n", ret);
        return AVERROR_EXTERNAL;
    }
    for (;;) {
        if (bytestream2_get_bytes_left(&s->gb) <= 0) {
            av_log(avctx, AV_LOG_ERROR, "%d bytes left\n", bytestream2_get_bytes_left(&s->gb));
            if (   s->state & PNG_ALLIMAGE
                && avctx->strict_std_compliance <= FF_COMPLIANCE_NORMAL)
                goto exit_loop;
            goto fail;
        }

        length = bytestream2_get_be32(&s->gb);
        if (length > 0x7fffffff || length > bytestream2_get_bytes_left(&s->gb))  {
            av_log(avctx, AV_LOG_ERROR, "chunk too big\n");
            goto fail;
        }
        tag = bytestream2_get_le32(&s->gb);
        if (avctx->debug & FF_DEBUG_STARTCODE)
            av_log(avctx, AV_LOG_DEBUG, "png: tag=%c%c%c%c length=%u\n",
                (tag & 0xff),
                ((tag >> 8) & 0xff),
                ((tag >> 16) & 0xff),
                ((tag >> 24) & 0xff), length);
        switch (tag) {
        case MKTAG('I', 'H', 'D', 'R'):
            if (length != 13)
                goto fail;
            s->width  = bytestream2_get_be32(&s->gb);
            s->height = bytestream2_get_be32(&s->gb);
            if (av_image_check_size(s->width, s->height, 0, avctx)) {
                s->width = s->height = 0;
                av_log(avctx, AV_LOG_ERROR, "Invalid image size\n");
                goto fail;
            }
            s->bit_depth        = bytestream2_get_byte(&s->gb);
            s->color_type       = bytestream2_get_byte(&s->gb);
            s->compression_type = bytestream2_get_byte(&s->gb);
            s->filter_type      = bytestream2_get_byte(&s->gb);
            s->interlace_type   = bytestream2_get_byte(&s->gb);
            bytestream2_skip(&s->gb, 4); /* crc */
            s->state |= PNG_IHDR;
            if (avctx->debug & FF_DEBUG_PICT_INFO)
                av_log(avctx, AV_LOG_DEBUG, "width=%d height=%d depth=%d color_type=%d "
                           "compression_type=%d filter_type=%d interlace_type=%d\n",
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
                    s->width, s->height, s->bit_depth, s->color_type,
                    s->compression_type, s->filter_type, s->interlace_type);
            break;
        case MKTAG('p', 'H', 'Y', 's'):
            if (s->state & PNG_IDAT) {
                av_log(avctx, AV_LOG_ERROR, "pHYs after IDAT\n");
                goto fail;
            }
            avctx->sample_aspect_ratio.num = bytestream2_get_be32(&s->gb);
            avctx->sample_aspect_ratio.den = bytestream2_get_be32(&s->gb);
            if (avctx->sample_aspect_ratio.num < 0 || avctx->sample_aspect_ratio.den < 0)
                avctx->sample_aspect_ratio = (AVRational){ 0, 1 };
            bytestream2_skip(&s->gb, 1); /* unit specifier */
            bytestream2_skip(&s->gb, 4); /* crc */
            break;
        case MKTAG('I', 'D', 'A', 'T'):
            if (!(s->state & PNG_IHDR)) {
                av_log(avctx, AV_LOG_ERROR, "IDAT without IHDR\n");
                goto fail;
            }
            if (!(s->state & PNG_IDAT)) {
                /* init image info */
                avctx->width  = s->width;
                avctx->height = s->height;

                s->channels       = ff_png_get_nb_channels(s->color_type);
                s->bits_per_pixel = s->bit_depth * s->channels;
                s->bpp            = (s->bits_per_pixel + 7) >> 3;
                s->row_size       = (avctx->width * s->bits_per_pixel + 7) >> 3;

                if ((s->bit_depth == 2 || s->bit_depth == 4 || s->bit_depth == 8) &&
                    s->color_type == PNG_COLOR_TYPE_RGB) {
                    avctx->pix_fmt = AV_PIX_FMT_RGB24;
                } else if ((s->bit_depth == 2 || s->bit_depth == 4 || s->bit_depth == 8) &&
                           s->color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                    avctx->pix_fmt = AV_PIX_FMT_RGBA;
                } else if ((s->bit_depth == 2 || s->bit_depth == 4 || s->bit_depth == 8) &&
                           s->color_type == PNG_COLOR_TYPE_GRAY) {
                    avctx->pix_fmt = AV_PIX_FMT_GRAY8;
                } else if (s->bit_depth == 16 &&
                           s->color_type == PNG_COLOR_TYPE_GRAY) {
                    avctx->pix_fmt = AV_PIX_FMT_GRAY16BE;
                } else if (s->bit_depth == 16 &&
                           s->color_type == PNG_COLOR_TYPE_RGB) {
                    avctx->pix_fmt = AV_PIX_FMT_RGB48BE;
                } else if (s->bit_depth == 16 &&
                           s->color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                    avctx->pix_fmt = AV_PIX_FMT_RGBA64BE;
                } else if ((s->bits_per_pixel == 1 || s->bits_per_pixel == 2 || s->bits_per_pixel == 4 || s->bits_per_pixel == 8) &&
                           s->color_type == PNG_COLOR_TYPE_PALETTE) {
                    avctx->pix_fmt = AV_PIX_FMT_PAL8;
                } else if (s->bit_depth == 1) {
                    avctx->pix_fmt = AV_PIX_FMT_MONOBLACK;
                } else if (s->bit_depth == 8 &&
                           s->color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                    avctx->pix_fmt = AV_PIX_FMT_YA8;
                } else if (s->bit_depth == 16 &&
                           s->color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                    avctx->pix_fmt = AV_PIX_FMT_YA16BE;
                } else {
                    av_log(avctx, AV_LOG_ERROR, "unsupported bit depth %d "
                                                "and color type %d\n",
                                                 s->bit_depth, s->color_type);
                    goto fail;
                }

                if (ff_thread_get_buffer(avctx, &s->picture, AV_GET_BUFFER_FLAG_REF) < 0)
                    goto fail;
                ff_thread_finish_setup(avctx);

                p->pict_type        = AV_PICTURE_TYPE_I;
                p->key_frame        = 1;
                p->interlaced_frame = !!s->interlace_type;

                /* compute the compressed row size */
                if (!s->interlace_type) {
                    s->crow_size = s->row_size + 1;
                } else {
                    s->pass          = 0;
                    s->pass_row_size = ff_png_pass_row_size(s->pass,
                                                            s->bits_per_pixel,
                                                            s->width);
                    s->crow_size = s->pass_row_size + 1;
                }
                av_dlog(avctx, "row_size=%d crow_size =%d\n",
                        s->row_size, s->crow_size);
                s->image_buf      = p->data[0];
                s->image_linesize = p->linesize[0];
                /* copy the palette if needed */
                if (avctx->pix_fmt == AV_PIX_FMT_PAL8)
                    memcpy(p->data[1], s->palette, 256 * sizeof(uint32_t));
                /* empty row is used if differencing to the first row */
                av_fast_padded_mallocz(&s->last_row, &s->last_row_size, s->row_size);
                if (!s->last_row)
                    goto fail;
                if (s->interlace_type ||
                    s->color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                    av_fast_padded_malloc(&s->tmp_row, &s->tmp_row_size, s->row_size);
                    if (!s->tmp_row)
                        goto fail;
                }
                /* compressed row */
                av_fast_padded_malloc(&s->buffer, &s->buffer_size, s->row_size + 16);
                if (!s->buffer)
                    goto fail;

                /* we want crow_buf+1 to be 16-byte aligned */
                s->crow_buf          = s->buffer + 15;
                s->zstream.avail_out = s->crow_size;
                s->zstream.next_out  = s->crow_buf;
            }
            s->state |= PNG_IDAT;
            if (png_decode_idat(s, length) < 0)
                goto fail;
            bytestream2_skip(&s->gb, 4); /* crc */
            break;
        case MKTAG('P', 'L', 'T', 'E'):
        {
            int n, i, r, g, b;

            if ((length % 3) != 0 || length > 256 * 3)
                goto skip_tag;
            /* read the palette */
            n = length / 3;
            for (i = 0; i < n; i++) {
                r = bytestream2_get_byte(&s->gb);
                g = bytestream2_get_byte(&s->gb);
                b = bytestream2_get_byte(&s->gb);
                s->palette[i] = (0xFFU << 24) | (r << 16) | (g << 8) | b;
            }
            for (; i < 256; i++)
                s->palette[i] = (0xFFU << 24);
            s->state |= PNG_PLTE;
            bytestream2_skip(&s->gb, 4);     /* crc */
        }
        break;
        case MKTAG('t', 'R', 'N', 'S'):
        {
            int v, i;

            /* read the transparency. XXX: Only palette mode supported */
            if (s->color_type != PNG_COLOR_TYPE_PALETTE ||
                length > 256 ||
                !(s->state & PNG_PLTE))
                goto skip_tag;
            for (i = 0; i < length; i++) {
                v = bytestream2_get_byte(&s->gb);
                s->palette[i] = (s->palette[i] & 0x00ffffff) | (v << 24);
            }
            bytestream2_skip(&s->gb, 4);     /* crc */
        }
        break;
        case MKTAG('t', 'E', 'X', 't'):
            if (decode_text_chunk(s, length, 0, &metadata) < 0)
                av_log(avctx, AV_LOG_WARNING, "Broken tEXt chunk\n");
            bytestream2_skip(&s->gb, length + 4);
            break;
        case MKTAG('z', 'T', 'X', 't'):
            if (decode_text_chunk(s, length, 1, &metadata) < 0)
                av_log(avctx, AV_LOG_WARNING, "Broken zTXt chunk\n");
            bytestream2_skip(&s->gb, length + 4);
            break;
        case MKTAG('I', 'E', 'N', 'D'):
            if (!(s->state & PNG_ALLIMAGE))
                av_log(avctx, AV_LOG_ERROR, "IEND without all image\n");
            if (!(s->state & (PNG_ALLIMAGE|PNG_IDAT))) {
                goto fail;
            }
            bytestream2_skip(&s->gb, 4); /* crc */
            goto exit_loop;
        default:
            /* skip tag */
skip_tag:
            bytestream2_skip(&s->gb, length + 4);
            break;
        }
    }
exit_loop:

    if (s->bits_per_pixel == 1 && s->color_type == PNG_COLOR_TYPE_PALETTE){
        int i, j, k;
        uint8_t *pd = p->data[0];
        for (j = 0; j < s->height; j++) {
            i = s->width / 8;
            for (k = 7; k >= 1; k--)
                if ((s->width&7) >= k)
                    pd[8*i + k - 1] = (pd[i]>>8-k) & 1;
            for (i--; i >= 0; i--) {
                pd[8*i + 7]=  pd[i]     & 1;
                pd[8*i + 6]= (pd[i]>>1) & 1;
                pd[8*i + 5]= (pd[i]>>2) & 1;
                pd[8*i + 4]= (pd[i]>>3) & 1;
                pd[8*i + 3]= (pd[i]>>4) & 1;
                pd[8*i + 2]= (pd[i]>>5) & 1;
                pd[8*i + 1]= (pd[i]>>6) & 1;
                pd[8*i + 0]=  pd[i]>>7;
            }
            pd += s->image_linesize;
        }
    }
    if (s->bits_per_pixel == 2){
        int i, j;
        uint8_t *pd = p->data[0];
        for (j = 0; j < s->height; j++) {
            i = s->width / 4;
            if (s->color_type == PNG_COLOR_TYPE_PALETTE){
                if ((s->width&3) >= 3) pd[4*i + 2]= (pd[i] >> 2) & 3;
                if ((s->width&3) >= 2) pd[4*i + 1]= (pd[i] >> 4) & 3;
                if ((s->width&3) >= 1) pd[4*i + 0]=  pd[i] >> 6;
                for (i--; i >= 0; i--) {
                    pd[4*i + 3]=  pd[i]     & 3;
                    pd[4*i + 2]= (pd[i]>>2) & 3;
                    pd[4*i + 1]= (pd[i]>>4) & 3;
                    pd[4*i + 0]=  pd[i]>>6;
                }
            } else {
                if ((s->width&3) >= 3) pd[4*i + 2]= ((pd[i]>>2) & 3)*0x55;
                if ((s->width&3) >= 2) pd[4*i + 1]= ((pd[i]>>4) & 3)*0x55;
                if ((s->width&3) >= 1) pd[4*i + 0]= ( pd[i]>>6     )*0x55;
                for (i--; i >= 0; i--) {
                    pd[4*i + 3]= ( pd[i]     & 3)*0x55;
                    pd[4*i + 2]= ((pd[i]>>2) & 3)*0x55;
                    pd[4*i + 1]= ((pd[i]>>4) & 3)*0x55;
                    pd[4*i + 0]= ( pd[i]>>6     )*0x55;
                }
            }
            pd += s->image_linesize;
        }
    }
    if (s->bits_per_pixel == 4){
        int i, j;
        uint8_t *pd = p->data[0];
        for (j = 0; j < s->height; j++) {
            i = s->width/2;
            if (s->color_type == PNG_COLOR_TYPE_PALETTE){
                if (s->width&1) pd[2*i+0]= pd[i]>>4;
                for (i--; i >= 0; i--) {
                pd[2*i + 1] = pd[i] & 15;
                pd[2*i + 0] = pd[i] >> 4;
            }
            } else {
                if (s->width & 1) pd[2*i + 0]= (pd[i] >> 4) * 0x11;
                for (i--; i >= 0; i--) {
                    pd[2*i + 1] = (pd[i] & 15) * 0x11;
                    pd[2*i + 0] = (pd[i] >> 4) * 0x11;
                }
            }
            pd += s->image_linesize;
        }
    }

    /* handle p-frames only if a predecessor frame is available */
    if (s->last_picture.f->data[0]) {
        if (   !(avpkt->flags & AV_PKT_FLAG_KEY) && avctx->codec_tag != AV_RL32("MPNG")
            && s->last_picture.f->width == p->width
            && s->last_picture.f->height== p->height
            && s->last_picture.f->format== p->format
         ) {
            int i, j;
            uint8_t *pd      = p->data[0];
            uint8_t *pd_last = s->last_picture.f->data[0];

            ff_thread_await_progress(&s->last_picture, INT_MAX, 0);
            for (j = 0; j < s->height; j++) {
                for (i = 0; i < s->width * s->bpp; i++)
                    pd[i] += pd_last[i];
                pd      += s->image_linesize;
                pd_last += s->image_linesize;
            }
        }
    }
    ff_thread_report_progress(&s->picture, INT_MAX, 0);

    av_frame_set_metadata(p, metadata);
    metadata   = NULL;

    if ((ret = av_frame_ref(data, s->picture.f)) < 0)
        return ret;

    *got_frame = 1;

    ret = bytestream2_tell(&s->gb);
the_end:
    inflateEnd(&s->zstream);
    s->crow_buf = NULL;
    return ret;
fail:
    av_dict_free(&metadata);
    ff_thread_report_progress(&s->picture, INT_MAX, 0);
    ret = AVERROR_INVALIDDATA;
    goto the_end;
}
