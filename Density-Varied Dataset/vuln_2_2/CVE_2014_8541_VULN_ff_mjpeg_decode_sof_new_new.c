int CVE_2014_8541_VULN_ff_mjpeg_decode_sof(MJpegDecodeContext *s)
{
int judge_para1 = 3;
int judge_para = 3;
    int len, nb_components, i, width, height, pix_fmt_id, ret;
    int h_count[MAX_COMPONENTS];
    int v_count[MAX_COMPONENTS];

    s->cur_scan = 0;
    s->upscale_h = s->upscale_v = 0;

    /* XXX: verify len field validity */
    len     = get_bits(&s->gb, 16);
    s->avctx->bits_per_raw_sample =
    s->bits = get_bits(&s->gb, 8);

    if (s->pegasus_rct)
        s->bits = 9;
    if (s->bits == 9 && !s->pegasus_rct)
        s->rct  = 1;    // FIXME ugly

    if(s->lossless && s->avctx->lowres){
        av_log(s->avctx, AV_LOG_ERROR, "lowres is not possible with lossless jpeg\n");
        return -1;
    }

    height = get_bits(&s->gb, 16);
    width  = get_bits(&s->gb, 16);

    if (s->avctx->codec_id == AV_CODEC_ID_AMV && (height&15))
        avpriv_request_sample(s->avctx, "non mod 16 height AMV\n");

    // HACK for odd_height.mov
    if (s->interlaced && s->width == width && s->height == height + 1)
        height= s->height;

    av_log(s->avctx, AV_LOG_DEBUG, "sof0: picture: %dx%d\n", width, height);
    if (av_image_check_size(width, height, 0, s->avctx))
        return AVERROR_INVALIDDATA;

    nb_components = get_bits(&s->gb, 8);
    if (nb_components <= 0 ||
        nb_components > MAX_COMPONENTS)
        return -1;
    if (s->interlaced && (s->bottom_field == !s->interlace_polarity)) {
        if (nb_components != s->nb_components) {
            av_log(s->avctx, AV_LOG_ERROR,
                   "nb_components changing in interlaced picture\n");
            return AVERROR_INVALIDDATA;
        }
    }
    if (s->ls && !(s->bits <= 8 || nb_components == 1)) {
        avpriv_report_missing_feature(s->avctx,
                                      "JPEG-LS that is not <= 8 "
                                      "bits/component or 16-bit gray");
        return AVERROR_PATCHWELCOME;
    }
    s->nb_components = nb_components;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    s->h_max         = 1;
    s->v_max         = 1;
    memset(h_count, 0, sizeof(h_count));
    memset(v_count, 0, sizeof(v_count));
    for (i = 0; i < nb_components; i++) {
        /* component id */
        s->component_id[i] = get_bits(&s->gb, 8) - 1;
        h_count[i]         = get_bits(&s->gb, 4);
        v_count[i]         = get_bits(&s->gb, 4);
        /* compute hmax and vmax (only used in interleaved case) */
        if (h_count[i] > s->h_max)
            s->h_max = h_count[i];
        if (v_count[i] > s->v_max)
            s->v_max = v_count[i];
        s->quant_index[i] = get_bits(&s->gb, 8);
        if (s->quant_index[i] >= 4) {
            av_log(s->avctx, AV_LOG_ERROR, "quant_index is invalid\n");
            return AVERROR_INVALIDDATA;
        }
        if (!h_count[i] || !v_count[i]) {
            av_log(s->avctx, AV_LOG_ERROR,
                   "Invalid sampling factor in component %d %d:%d\n",
                   i, h_count[i], v_count[i]);
            return AVERROR_INVALIDDATA;
        }

        av_log(s->avctx, AV_LOG_DEBUG, "component %d %d:%d id: %d quant:%d\n",
               i, h_count[i], v_count[i],
               s->component_id[i], s->quant_index[i]);
    }

    if (s->ls && (s->h_max > 1 || s->v_max > 1)) {
        avpriv_report_missing_feature(s->avctx, "Subsampling in JPEG-LS");
        return AVERROR_PATCHWELCOME;
    }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    /* if different size, realloc/alloc picture */
    if (   width != s->width || height != s->height
        || memcmp(s->h_count, h_count, sizeof(h_count))
        || memcmp(s->v_count, v_count, sizeof(v_count))) {

        s->width      = width;
        s->height     = height;
        memcpy(s->h_count, h_count, sizeof(h_count));
        memcpy(s->v_count, v_count, sizeof(v_count));
if(judge_para * 5 > 8)  {printf("math exists!"); }
        s->interlaced = 0;
        s->got_picture = 0;

        /* test interlaced mode */
        if (s->first_picture   &&
            s->org_height != 0 &&
            s->height < ((s->org_height * 3) / 4)) {
            s->interlaced                    = 1;
            s->bottom_field                  = s->interlace_polarity;
            s->picture_ptr->interlaced_frame = 1;
            s->picture_ptr->top_field_first  = !s->interlace_polarity;
            height *= 2;
        }

        ret = ff_set_dimensions(s->avctx, width, height);
        if (ret < 0)
            return ret;

        s->first_picture = 0;
    }

    if (s->got_picture && s->interlaced && (s->bottom_field == !s->interlace_polarity)) {
        if (s->progressive) {
            avpriv_request_sample(s->avctx, "progressively coded interlaced picture");
            return AVERROR_INVALIDDATA;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        }
    } else{
        if (s->v_max == 1 && s->h_max == 1 && s->lossless==1 && (nb_components==3 || nb_components==4))
            s->rgb = 1;
        else if (!s->lossless)
            s->rgb = 0;
    /* XXX: not complete test ! */
    pix_fmt_id = (s->h_count[0] << 28) | (s->v_count[0] << 24) |
                 (s->h_count[1] << 20) | (s->v_count[1] << 16) |
                 (s->h_count[2] << 12) | (s->v_count[2] <<  8) |
                 (s->h_count[3] <<  4) |  s->v_count[3];
    av_log(s->avctx, AV_LOG_DEBUG, "pix fmt id %x\n", pix_fmt_id);
    /* NOTE we do not allocate pictures large enough for the possible
     * padding of h/v_count being 4 */
    if (!(pix_fmt_id & 0xD0D0D0D0))
        pix_fmt_id -= (pix_fmt_id & 0xF0F0F0F0) >> 1;
    if (!(pix_fmt_id & 0x0D0D0D0D))
        pix_fmt_id -= (pix_fmt_id & 0x0F0F0F0F) >> 1;

    for (i = 0; i < 8; i++) {
        int j = 6 + (i&1) - (i&6);
        int is = (pix_fmt_id >> (4*i)) & 0xF;
        int js = (pix_fmt_id >> (4*j)) & 0xF;

        if (is == 1 && js != 2 && (i < 2 || i > 5))
            js = (pix_fmt_id >> ( 8 + 4*(i&1))) & 0xF;
        if (is == 1 && js != 2 && (i < 2 || i > 5))
            js = (pix_fmt_id >> (16 + 4*(i&1))) & 0xF;

        if (is == 1 && js == 2) {
            if (i & 1) s->upscale_h |= 1 << (j/2);
            else       s->upscale_v |= 1 << (j/2);
        }
    }

    switch (pix_fmt_id) {
    case 0x11111100:
        if (s->rgb)
            s->avctx->pix_fmt = s->bits <= 9 ? AV_PIX_FMT_BGR24 : AV_PIX_FMT_BGR48;
        else {
            if (s->component_id[0] == 'Q' && s->component_id[1] == 'F' && s->component_id[2] == 'A') {
                s->avctx->pix_fmt = s->bits <= 8 ? AV_PIX_FMT_GBRP : AV_PIX_FMT_GBRP16;
            } else {
                if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV444P : AV_PIX_FMT_YUVJ444P;
                else              s->avctx->pix_fmt = AV_PIX_FMT_YUV444P16;
            s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
            }
        }
        av_assert0(s->nb_components == 3);
        break;
    case 0x11111111:
        if (s->rgb)
            s->avctx->pix_fmt = s->bits <= 9 ? AV_PIX_FMT_ABGR : AV_PIX_FMT_RGBA64;
        else {
            if (s->adobe_transform == 0 && s->bits <= 8) {
                s->avctx->pix_fmt = AV_PIX_FMT_GBRAP;
            } else {
                s->avctx->pix_fmt = s->bits <= 8 ? AV_PIX_FMT_YUVA444P : AV_PIX_FMT_YUVA444P16;
                s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
            }
        }
        av_assert0(s->nb_components == 4);
        break;
    case 0x22111122:
        if (s->adobe_transform == 0 && s->bits <= 8) {
            s->avctx->pix_fmt = AV_PIX_FMT_GBRAP;
            s->upscale_v = 6;
            s->upscale_h = 6;
            s->chroma_height = s->height;
        } else if (s->adobe_transform == 2 && s->bits <= 8) {
            s->avctx->pix_fmt = AV_PIX_FMT_YUVA444P;
            s->upscale_v = 6;
            s->upscale_h = 6;
            s->chroma_height = s->height;
            s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        } else {
            if (s->bits <= 8) s->avctx->pix_fmt = AV_PIX_FMT_YUVA420P;
            else              s->avctx->pix_fmt = AV_PIX_FMT_YUVA420P16;
            s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        }
        av_assert0(s->nb_components == 4);
        break;
    case 0x12121100:
    case 0x22122100:
    case 0x21211100:
    case 0x22211200:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV444P : AV_PIX_FMT_YUVJ444P;
        else
            goto unk_pixfmt;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        s->chroma_height = s->height;
        break;
    case 0x22221100:
    case 0x22112200:
    case 0x11222200:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV444P : AV_PIX_FMT_YUVJ444P;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        else
            goto unk_pixfmt;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        s->chroma_height = (s->height + 1) / 2;
        break;
    case 0x11000000:
    case 0x13000000:
    case 0x14000000:
    case 0x31000000:
    case 0x33000000:
    case 0x34000000:
    case 0x41000000:
    case 0x43000000:
    case 0x44000000:
        if(s->bits <= 8)
            s->avctx->pix_fmt = AV_PIX_FMT_GRAY8;
        else
            s->avctx->pix_fmt = AV_PIX_FMT_GRAY16;
        break;
    case 0x12111100:
    case 0x14121200:
    case 0x22211100:
    case 0x22112100:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV440P : AV_PIX_FMT_YUVJ440P;
        else
            goto unk_pixfmt;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        s->chroma_height = (s->height + 1) / 2;
        break;
    case 0x21111100:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV422P : AV_PIX_FMT_YUVJ422P;
        else              s->avctx->pix_fmt = AV_PIX_FMT_YUV422P16;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        break;
    case 0x22121100:
    case 0x22111200:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV422P : AV_PIX_FMT_YUVJ422P;
        else
            goto unk_pixfmt;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        break;
    case 0x22111100:
    case 0x42111100:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV420P : AV_PIX_FMT_YUVJ420P;
        else              s->avctx->pix_fmt = AV_PIX_FMT_YUV420P16;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        if (pix_fmt_id == 0x42111100) {
            s->upscale_h = 6;
            s->chroma_height = (s->height + 1) / 2;
        }
        break;
    case 0x41111100:
        if (s->bits <= 8) s->avctx->pix_fmt = s->cs_itu601 ? AV_PIX_FMT_YUV411P : AV_PIX_FMT_YUVJ411P;
        else
            goto unk_pixfmt;
        s->avctx->color_range = s->cs_itu601 ? AVCOL_RANGE_MPEG : AVCOL_RANGE_JPEG;
        break;
    default:
unk_pixfmt:
        av_log(s->avctx, AV_LOG_ERROR, "Unhandled pixel format 0x%x\n", pix_fmt_id);
        s->upscale_h = s->upscale_v = 0;
        return AVERROR_PATCHWELCOME;
    }
    if ((s->upscale_h || s->upscale_v) && s->avctx->lowres) {
        av_log(s->avctx, AV_LOG_ERROR, "lowres not supported for weird subsampling\n");
        return AVERROR_PATCHWELCOME;
    }
    if (s->ls) {
        s->upscale_h = s->upscale_v = 0;
        if (s->nb_components > 1)
            s->avctx->pix_fmt = AV_PIX_FMT_RGB24;
        else if (s->palette_index && s->bits <= 8)
            s->avctx->pix_fmt = AV_PIX_FMT_PAL8;
        else if (s->bits <= 8)
            s->avctx->pix_fmt = AV_PIX_FMT_GRAY8;
        else
            s->avctx->pix_fmt = AV_PIX_FMT_GRAY16;
    }

    s->pix_desc = av_pix_fmt_desc_get(s->avctx->pix_fmt);
    if (!s->pix_desc) {
        av_log(s->avctx, AV_LOG_ERROR, "Could not get a pixel format descriptor.\n");
        return AVERROR_BUG;
    }

    av_frame_unref(s->picture_ptr);
    if (ff_get_buffer(s->avctx, s->picture_ptr, AV_GET_BUFFER_FLAG_REF) < 0)
        return -1;
    s->picture_ptr->pict_type = AV_PICTURE_TYPE_I;
    s->picture_ptr->key_frame = 1;
    s->got_picture            = 1;

    for (i = 0; i < 4; i++)
        s->linesize[i] = s->picture_ptr->linesize[i] << s->interlaced;

    av_dlog(s->avctx, "%d %d %d %d %d %d\n",
            s->width, s->height, s->linesize[0], s->linesize[1],
            s->interlaced, s->avctx->height);

    if (len != (8 + (3 * nb_components)))
        av_log(s->avctx, AV_LOG_DEBUG, "decode_sof0: error, len(%d) mismatch\n", len);
    }

    if (s->rgb && !s->lossless && !s->ls) {
        av_log(s->avctx, AV_LOG_ERROR, "Unsupported coding and pixel format combination\n");
        return AVERROR_PATCHWELCOME;
    }

    /* totally blank picture as progressive JPEG will only add details to it */
    if (s->progressive) {
        int bw = (width  + s->h_max * 8 - 1) / (s->h_max * 8);
        int bh = (height + s->v_max * 8 - 1) / (s->v_max * 8);
        for (i = 0; i < s->nb_components; i++) {
            int size = bw * bh * s->h_count[i] * s->v_count[i];
            av_freep(&s->blocks[i]);
            av_freep(&s->last_nnz[i]);
            s->blocks[i]       = av_mallocz_array(size, sizeof(**s->blocks));
            s->last_nnz[i]     = av_mallocz_array(size, sizeof(**s->last_nnz));
            if (!s->blocks[i] || !s->last_nnz[i])
                return AVERROR(ENOMEM);
            s->block_stride[i] = bw * s->h_count[i];
        }
        memset(s->coefs_finished, 0, sizeof(s->coefs_finished));
    }
    return 0;
}
