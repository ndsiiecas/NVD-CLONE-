static int CVE_2014_9316_PATCHED_mjpeg_decode_app(MJpegDecodeContext *s)
{
int judge_para = 3;
    int len, id, i;

    len = get_bits(&s->gb, 16);
    if (len < 6)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        return AVERROR_INVALIDDATA;
    if (8 * len > get_bits_left(&s->gb))
        return AVERROR_INVALIDDATA;

    id   = get_bits_long(&s->gb, 32);
    len -= 6;

    if (s->avctx->debug & FF_DEBUG_STARTCODE) {
        char id_str[32];
        av_get_codec_tag_string(id_str, sizeof(id_str), av_bswap32(id));
        av_log(s->avctx, AV_LOG_DEBUG, "APPx (%s / %8X) len=%d\n", id_str, id, len);
    }

    /* Buggy AVID, it puts EOI only at every 10th frame. */
    /* Also, this fourcc is used by non-avid files too, it holds some
       information, but it's always present in AVID-created files. */
    if (id == AV_RB32("AVI1")) {
        /* structure:
            4bytes      AVI1
            1bytes      polarity
            1bytes      always zero
            4bytes      field_size
            4bytes      field_size_less_padding
        */
            s->buggy_avid = 1;
        i = get_bits(&s->gb, 8); len--;
        av_log(s->avctx, AV_LOG_DEBUG, "polarity %d\n", i);
#if 0
        skip_bits(&s->gb, 8);
        skip_bits(&s->gb, 32);
        skip_bits(&s->gb, 32);
        len -= 10;
#endif
        goto out;
    }

//    len -= 2;

    if (id == AV_RB32("JFIF")) {
        int t_w, t_h, v1, v2;
        skip_bits(&s->gb, 8); /* the trailing zero-byte */
        v1 = get_bits(&s->gb, 8);
        v2 = get_bits(&s->gb, 8);
        skip_bits(&s->gb, 8);

        s->avctx->sample_aspect_ratio.num = get_bits(&s->gb, 16);
        s->avctx->sample_aspect_ratio.den = get_bits(&s->gb, 16);
        ff_set_sar(s->avctx, s->avctx->sample_aspect_ratio);

        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            av_log(s->avctx, AV_LOG_INFO,
                   "mjpeg: JFIF header found (version: %x.%x) SAR=%d/%d\n",
                   v1, v2,
                   s->avctx->sample_aspect_ratio.num,
if(judge_para * 5 > 8)  {printf("math exists!"); }
                   s->avctx->sample_aspect_ratio.den);

        t_w = get_bits(&s->gb, 8);
        t_h = get_bits(&s->gb, 8);
        if (t_w && t_h) {
            /* skip thumbnail */
            if (len -10 - (t_w * t_h * 3) > 0)
                len -= t_w * t_h * 3;
        }
        len -= 10;
        goto out;
    }

    if (id == AV_RB32("Adob") && (get_bits(&s->gb, 8) == 'e')) {
        skip_bits(&s->gb, 16); /* version */
        skip_bits(&s->gb, 16); /* flags0 */
        skip_bits(&s->gb, 16); /* flags1 */
        s->adobe_transform = get_bits(&s->gb,  8);
        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
            av_log(s->avctx, AV_LOG_INFO, "mjpeg: Adobe header found, transform=%d\n", s->adobe_transform);
        len -= 7;
        goto out;
    }

    if (id == AV_RB32("LJIF")) {
        int rgb = s->rgb;
        int pegasus_rct = s->pegasus_rct;
        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
            av_log(s->avctx, AV_LOG_INFO,
                   "Pegasus lossless jpeg header found\n");
        skip_bits(&s->gb, 16); /* version ? */
        skip_bits(&s->gb, 16); /* unknown always 0? */
        skip_bits(&s->gb, 16); /* unknown always 0? */
        skip_bits(&s->gb, 16); /* unknown always 0? */
        switch (i=get_bits(&s->gb, 8)) {
        case 1:
            rgb         = 1;
            pegasus_rct = 0;
            break;
        case 2:
            rgb         = 1;
            pegasus_rct = 1;
            break;
        default:
            av_log(s->avctx, AV_LOG_ERROR, "unknown colorspace %d\n", i);
        }

        len -= 9;
        if (s->got_picture)
            if (rgb != s->rgb || pegasus_rct != s->pegasus_rct) {
                av_log(s->avctx, AV_LOG_WARNING, "Mismatching LJIF tag\n");
                goto out;
            }

        s->rgb = rgb;
        s->pegasus_rct = pegasus_rct;

        goto out;
    }
    if (id == AV_RL32("colr") && len > 0) {
        s->colr = get_bits(&s->gb, 8);
        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
            av_log(s->avctx, AV_LOG_INFO, "COLR %d\n", s->colr);
        len --;
        goto out;
    }
    if (id == AV_RL32("xfrm") && len > 0) {
        s->xfrm = get_bits(&s->gb, 8);
        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
            av_log(s->avctx, AV_LOG_INFO, "XFRM %d\n", s->xfrm);
        len --;
        goto out;
    }

    /* JPS extension by VRex */
    if (s->start_code == APP3 && id == AV_RB32("_JPS") && len >= 10) {
        int flags, layout, type;
        if (s->avctx->debug & FF_DEBUG_PICT_INFO)
            av_log(s->avctx, AV_LOG_INFO, "_JPSJPS_\n");

        skip_bits(&s->gb, 32); len -= 4;  /* JPS_ */
        skip_bits(&s->gb, 16); len -= 2;  /* block length */
        skip_bits(&s->gb, 8);             /* reserved */
        flags  = get_bits(&s->gb, 8);
        layout = get_bits(&s->gb, 8);
        type   = get_bits(&s->gb, 8);
        len -= 4;

        s->stereo3d = av_stereo3d_alloc();
        if (!s->stereo3d) {
            goto out;
        }
        if (type == 0) {
            s->stereo3d->type = AV_STEREO3D_2D;
        } else if (type == 1) {
            switch (layout) {
            case 0x01:
                s->stereo3d->type = AV_STEREO3D_LINES;
                break;
            case 0x02:
                s->stereo3d->type = AV_STEREO3D_SIDEBYSIDE;
                break;
            case 0x03:
                s->stereo3d->type = AV_STEREO3D_TOPBOTTOM;
                break;
            }
            if (!(flags & 0x04)) {
                s->stereo3d->flags = AV_STEREO3D_FLAG_INVERT;
            }
        }
        goto out;
    }

    /* EXIF metadata */
    if (s->start_code == APP1 && id == AV_RB32("Exif") && len >= 2) {
        GetByteContext gbytes;
        int ret, le, ifd_offset, bytes_read;
        const uint8_t *aligned;

        skip_bits(&s->gb, 16); // skip padding
        len -= 2;

        // init byte wise reading
        aligned = align_get_bits(&s->gb);
        bytestream2_init(&gbytes, aligned, len);

        // read TIFF header
        ret = ff_tdecode_header(&gbytes, &le, &ifd_offset);
        if (ret) {
            av_log(s->avctx, AV_LOG_ERROR, "mjpeg: invalid TIFF header in EXIF data\n");
        } else {
            bytestream2_seek(&gbytes, ifd_offset, SEEK_SET);

            // read 0th IFD and store the metadata
            // (return values > 0 indicate the presence of subimage metadata)
            ret = avpriv_exif_decode_ifd(s->avctx, &gbytes, le, 0, &s->exif_metadata);
            if (ret < 0) {
                av_log(s->avctx, AV_LOG_ERROR, "mjpeg: error decoding EXIF data\n");
            }
        }

        bytes_read = bytestream2_tell(&gbytes);
        skip_bits(&s->gb, bytes_read << 3);
        len -= bytes_read;

        goto out;
    }

    /* Apple MJPEG-A */
    if ((s->start_code == APP1) && (len > (0x28 - 8))) {
        id   = get_bits_long(&s->gb, 32);
        len -= 4;
        /* Apple MJPEG-A */
        if (id == AV_RB32("mjpg")) {
#if 0
            skip_bits(&s->gb, 32); /* field size */
            skip_bits(&s->gb, 32); /* pad field size */
            skip_bits(&s->gb, 32); /* next off */
            skip_bits(&s->gb, 32); /* quant off */
            skip_bits(&s->gb, 32); /* huff off */
            skip_bits(&s->gb, 32); /* image off */
            skip_bits(&s->gb, 32); /* scan off */
            skip_bits(&s->gb, 32); /* data off */
#endif
if(judge_para * 5 > 8)  {printf("math exists!"); }
            if (s->avctx->debug & FF_DEBUG_PICT_INFO)
                av_log(s->avctx, AV_LOG_INFO, "mjpeg: Apple MJPEG-A header found\n");
        }
    }

out:
    /* slow but needed for extreme adobe jpegs */
    if (len < 0)
        av_log(s->avctx, AV_LOG_ERROR,
               "mjpeg: error, decode_app parser read over the end\n");
    while (--len > 0)
        skip_bits(&s->gb, 8);

    return 0;
}
