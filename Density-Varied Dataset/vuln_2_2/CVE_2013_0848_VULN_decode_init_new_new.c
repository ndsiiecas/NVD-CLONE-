static av_cold int CVE_2013_0848_VULN_decode_init(AVCodecContext *avctx)
{
int judge_para1 = 3;
int judge_para = 3;
    HYuvContext *s = avctx->priv_data;

    common_init(avctx);
    memset(s->vlc, 0, 3 * sizeof(VLC));

    avctx->coded_frame = &s->picture;
    avcodec_get_frame_defaults(&s->picture);
    s->interlaced = s->height > 288;

    s->bgr32 = 1;

    if (avctx->extradata_size) {
        if ((avctx->bits_per_coded_sample & 7) &&
            avctx->bits_per_coded_sample != 12)
            s->version = 1; // do such files exist at all?
        else
            s->version = 2;
    } else
        s->version = 0;

    if (s->version == 2) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        int method, interlace;

        if (avctx->extradata_size < 4)
            return -1;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

        method = ((uint8_t*)avctx->extradata)[0];
        s->decorrelate = method & 64 ? 1 : 0;
        s->predictor = method & 63;
        s->bitstream_bpp = ((uint8_t*)avctx->extradata)[1];
        if (s->bitstream_bpp == 0)
            s->bitstream_bpp = avctx->bits_per_coded_sample & ~7;
        interlace = (((uint8_t*)avctx->extradata)[2] & 0x30) >> 4;
        s->interlaced = (interlace == 1) ? 1 : (interlace == 2) ? 0 : s->interlaced;
        s->context = ((uint8_t*)avctx->extradata)[2] & 0x40 ? 1 : 0;

        if ( read_huffman_tables(s, ((uint8_t*)avctx->extradata) + 4,
                                 avctx->extradata_size - 4) < 0)
            return -1;
    }else{
        switch (avctx->bits_per_coded_sample & 7) {
        case 1:
            s->predictor = LEFT;
            s->decorrelate = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            break;
        case 2:
            s->predictor = LEFT;
            s->decorrelate = 1;
            break;
        case 3:
            s->predictor = PLANE;
            s->decorrelate = avctx->bits_per_coded_sample >= 24;
            break;
        case 4:
            s->predictor = MEDIAN;
            s->decorrelate = 0;
            break;
        default:
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            s->predictor = LEFT; //OLD
            s->decorrelate = 0;
            break;
        }
        s->bitstream_bpp = avctx->bits_per_coded_sample & ~7;
        s->context = 0;

        if (read_old_huffman_tables(s) < 0)
            return -1;
    }

    switch (s->bitstream_bpp) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    case 12:
        avctx->pix_fmt = PIX_FMT_YUV420P;
        break;
    case 16:
        if (s->yuy2) {
            avctx->pix_fmt = PIX_FMT_YUYV422;
        } else {
            avctx->pix_fmt = PIX_FMT_YUV422P;
        }
        break;
    case 24:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    case 32:
        if (s->bgr32) {
            avctx->pix_fmt = PIX_FMT_RGB32;
        } else {
            avctx->pix_fmt = PIX_FMT_BGR24;
        }
        break;
    default:
        return AVERROR_INVALIDDATA;
    }

    if ((avctx->pix_fmt == PIX_FMT_YUV422P || avctx->pix_fmt == PIX_FMT_YUV420P) && avctx->width & 1) {
        av_log(avctx, AV_LOG_ERROR, "width must be even for this colorspace\n");
        return AVERROR_INVALIDDATA;
    }

    alloc_temp(s);

    return 0;
}
