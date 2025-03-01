static int CVE_2013_7011_PATCHED_read_header(FFV1Context *f)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    uint8_t state[CONTEXT_SIZE];
    int i, j, context_count = -1; //-1 to avoid warning
    RangeCoder *const c = &f->slice_context[0]->c;

    memset(state, 128, sizeof(state));

    if (f->version < 2) {
        int chroma_planes, chroma_h_shift, chroma_v_shift, transparency;
        unsigned v= get_symbol(c, state, 0);
        if (v >= 2) {
            av_log(f->avctx, AV_LOG_ERROR, "invalid version %d in ver01 header\n", v);
            return AVERROR_INVALIDDATA;
        }
        f->version = v;
        f->ac      = f->avctx->coder_type = get_symbol(c, state, 0);
        if (f->ac > 1) {
            for (i = 1; i < 256; i++)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                f->state_transition[i] = get_symbol(c, state, 1) + c->one_state[i];
        }

        f->colorspace = get_symbol(c, state, 0); //YUV cs type

        if (f->version > 0)
            f->avctx->bits_per_raw_sample = get_symbol(c, state, 0);

        chroma_planes  = get_rac(c, state);
        chroma_h_shift = get_symbol(c, state, 0);
        chroma_v_shift = get_symbol(c, state, 0);
        transparency   = get_rac(c, state);

        if (f->plane_count) {
            if (   chroma_planes != f->chroma_planes
                || chroma_h_shift!= f->chroma_h_shift
                || chroma_v_shift!= f->chroma_v_shift
                || transparency  != f->transparency) {
                av_log(f->avctx, AV_LOG_ERROR, "Invalid change of global parameters\n");
                return AVERROR_INVALIDDATA;
            }
        }

        f->chroma_planes  = chroma_planes;
        f->chroma_h_shift = chroma_h_shift;
        f->chroma_v_shift = chroma_v_shift;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        f->transparency   = transparency;

        f->plane_count    = 2 + f->transparency;
    }

    if (f->colorspace == 0) {
        if (!f->transparency && !f->chroma_planes) {
            if (f->avctx->bits_per_raw_sample <= 8)
                f->avctx->pix_fmt = AV_PIX_FMT_GRAY8;
            else
                f->avctx->pix_fmt = AV_PIX_FMT_GRAY16;
        } else if (f->avctx->bits_per_raw_sample<=8 && !f->transparency) {
            switch(16 * f->chroma_h_shift + f->chroma_v_shift) {
            case 0x00: f->avctx->pix_fmt = AV_PIX_FMT_YUV444P; break;
            case 0x01: f->avctx->pix_fmt = AV_PIX_FMT_YUV440P; break;
            case 0x10: f->avctx->pix_fmt = AV_PIX_FMT_YUV422P; break;
            case 0x11: f->avctx->pix_fmt = AV_PIX_FMT_YUV420P; break;
            case 0x20: f->avctx->pix_fmt = AV_PIX_FMT_YUV411P; break;
            case 0x22: f->avctx->pix_fmt = AV_PIX_FMT_YUV410P; break;
            default:
                av_log(f->avctx, AV_LOG_ERROR, "format not supported\n");
                return AVERROR(ENOSYS);
            }
        } else if (f->avctx->bits_per_raw_sample <= 8 && f->transparency) {
            switch(16*f->chroma_h_shift + f->chroma_v_shift) {
            case 0x00: f->avctx->pix_fmt = AV_PIX_FMT_YUVA444P; break;
            case 0x10: f->avctx->pix_fmt = AV_PIX_FMT_YUVA422P; break;
            case 0x11: f->avctx->pix_fmt = AV_PIX_FMT_YUVA420P; break;
            default:
                av_log(f->avctx, AV_LOG_ERROR, "format not supported\n");
                return AVERROR(ENOSYS);
            }
        } else if (f->avctx->bits_per_raw_sample == 9) {
            f->packed_at_lsb = 1;
            switch(16 * f->chroma_h_shift + f->chroma_v_shift) {
            case 0x00: f->avctx->pix_fmt = AV_PIX_FMT_YUV444P9; break;
            case 0x10: f->avctx->pix_fmt = AV_PIX_FMT_YUV422P9; break;
            case 0x11: f->avctx->pix_fmt = AV_PIX_FMT_YUV420P9; break;
            default:
                av_log(f->avctx, AV_LOG_ERROR, "format not supported\n");
                return AVERROR(ENOSYS);
            }
        } else if (f->avctx->bits_per_raw_sample == 10) {
            f->packed_at_lsb = 1;
            switch(16 * f->chroma_h_shift + f->chroma_v_shift) {
            case 0x00: f->avctx->pix_fmt = AV_PIX_FMT_YUV444P10; break;
            case 0x10: f->avctx->pix_fmt = AV_PIX_FMT_YUV422P10; break;
            case 0x11: f->avctx->pix_fmt = AV_PIX_FMT_YUV420P10; break;
            default:
                av_log(f->avctx, AV_LOG_ERROR, "format not supported\n");
                return AVERROR(ENOSYS);
            }
        } else {
            switch(16 * f->chroma_h_shift + f->chroma_v_shift) {
            case 0x00: f->avctx->pix_fmt = AV_PIX_FMT_YUV444P16; break;
            case 0x10: f->avctx->pix_fmt = AV_PIX_FMT_YUV422P16; break;
            case 0x11: f->avctx->pix_fmt = AV_PIX_FMT_YUV420P16; break;
            default:
                av_log(f->avctx, AV_LOG_ERROR, "format not supported\n");
                return AVERROR(ENOSYS);
            }
        }
    } else if (f->colorspace == 1) {
        if (f->chroma_h_shift || f->chroma_v_shift) {
            av_log(f->avctx, AV_LOG_ERROR,
                   "chroma subsampling not supported in this colorspace\n");
            return AVERROR(ENOSYS);
        }
        if (     f->avctx->bits_per_raw_sample ==  9)
            f->avctx->pix_fmt = AV_PIX_FMT_GBRP9;
        else if (f->avctx->bits_per_raw_sample == 10)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            f->avctx->pix_fmt = AV_PIX_FMT_GBRP10;
        else if (f->avctx->bits_per_raw_sample == 12)
            f->avctx->pix_fmt = AV_PIX_FMT_GBRP12;
        else if (f->avctx->bits_per_raw_sample == 14)
            f->avctx->pix_fmt = AV_PIX_FMT_GBRP14;
        else
        if (f->transparency) f->avctx->pix_fmt = AV_PIX_FMT_RGB32;
        else                 f->avctx->pix_fmt = AV_PIX_FMT_0RGB32;
    } else {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        av_log(f->avctx, AV_LOG_ERROR, "colorspace not supported\n");
        return AVERROR(ENOSYS);
    }

    av_dlog(f->avctx, "%d %d %d\n",
            f->chroma_h_shift, f->chroma_v_shift, f->avctx->pix_fmt);
    if (f->version < 2) {
        context_count = read_quant_tables(c, f->quant_table);
        if (context_count < 0) {
            av_log(f->avctx, AV_LOG_ERROR, "read_quant_table error\n");
            return AVERROR_INVALIDDATA;
        }
    } else if (f->version < 3) {
        f->slice_count = get_symbol(c, state, 0);
    } else {
        const uint8_t *p = c->bytestream_end;
        for (f->slice_count = 0;
             f->slice_count < MAX_SLICES && 3 < p - c->bytestream_start;
             f->slice_count++) {
            int trailer = 3 + 5*!!f->ec;
            int size = AV_RB24(p-trailer);
            if (size + trailer > p - c->bytestream_start)
                break;
            p -= size + trailer;
        }
    }
    if (f->slice_count > (unsigned)MAX_SLICES || f->slice_count <= 0) {
        av_log(f->avctx, AV_LOG_ERROR, "slice count %d is invalid\n", f->slice_count);
        return AVERROR_INVALIDDATA;
    }

    for (j = 0; j < f->slice_count; j++) {
        FFV1Context *fs = f->slice_context[j];
        fs->ac            = f->ac;
        fs->packed_at_lsb = f->packed_at_lsb;

        fs->slice_damaged = 0;

        if (f->version == 2) {
            fs->slice_x      =  get_symbol(c, state, 0)      * f->width ;
            fs->slice_y      =  get_symbol(c, state, 0)      * f->height;
            fs->slice_width  = (get_symbol(c, state, 0) + 1) * f->width  + fs->slice_x;
            fs->slice_height = (get_symbol(c, state, 0) + 1) * f->height + fs->slice_y;

            fs->slice_x     /= f->num_h_slices;
            fs->slice_y     /= f->num_v_slices;
            fs->slice_width  = fs->slice_width  / f->num_h_slices - fs->slice_x;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            fs->slice_height = fs->slice_height / f->num_v_slices - fs->slice_y;
            if ((unsigned)fs->slice_width  > f->width ||
                (unsigned)fs->slice_height > f->height)
                return AVERROR_INVALIDDATA;
            if (   (unsigned)fs->slice_x + (uint64_t)fs->slice_width  > f->width
                || (unsigned)fs->slice_y + (uint64_t)fs->slice_height > f->height)
                return AVERROR_INVALIDDATA;
        }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        for (i = 0; i < f->plane_count; i++) {
            PlaneContext *const p = &fs->plane[i];

            if (f->version == 2) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
                int idx = get_symbol(c, state, 0);
                if (idx > (unsigned)f->quant_table_count) {
                    av_log(f->avctx, AV_LOG_ERROR,
                           "quant_table_index out of range\n");
                    return AVERROR_INVALIDDATA;
                }
                p->quant_table_index = idx;
                memcpy(p->quant_table, f->quant_tables[idx],
                       sizeof(p->quant_table));
                context_count = f->context_count[idx];
            } else {
                memcpy(p->quant_table, f->quant_table, sizeof(p->quant_table));
            }

            if (f->version <= 2) {
                av_assert0(context_count >= 0);
                if (p->context_count < context_count) {
                    av_freep(&p->state);
                    av_freep(&p->vlc_state);
                }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                p->context_count = context_count;
            }
        }
    }
    return 0;
}
