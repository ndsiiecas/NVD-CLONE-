int CVE_2012_2783_PATCHED_ff_vp56_decode_frame(AVCodecContext *avctx, void *data, int *data_size,
                         AVPacket *avpkt)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    const uint8_t *buf = avpkt->data;
    VP56Context *s = avctx->priv_data;
    AVFrame *const p = s->framep[VP56_FRAME_CURRENT];
    int remaining_buf_size = avpkt->size;
    int is_alpha, av_uninit(alpha_offset);

    if (s->has_alpha) {
        if (remaining_buf_size < 3)
            return -1;
        alpha_offset = bytestream_get_be24(&buf);
        remaining_buf_size -= 3;
        if (remaining_buf_size < alpha_offset)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            return -1;
    }

    for (is_alpha=0; is_alpha < 1+s->has_alpha; is_alpha++) {
        int mb_row, mb_col, mb_row_flip, mb_offset = 0;
        int block, y, uv, stride_y, stride_uv;
        int golden_frame = 0;
        int res;

        s->modelp = &s->models[is_alpha];

        res = s->parse_header(s, buf, remaining_buf_size, &golden_frame);
        if (!res)
            return -1;

        if (res == 2) {
            int i;
            for (i = 0; i < 4; i++) {
                if (s->frames[i].data[0])
                    avctx->release_buffer(avctx, &s->frames[i]);
            }
            if (is_alpha) {
                avcodec_set_dimensions(avctx, 0, 0);
                return -1;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            }
        }

        if (!is_alpha) {
            p->reference = 1;
            if (avctx->get_buffer(avctx, p) < 0) {
                av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
                return -1;
            }

            if (res == 2)
                if (vp56_size_changed(avctx)) {
                    avctx->release_buffer(avctx, p);
                    return -1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        if (p->key_frame) {
            p->pict_type = AV_PICTURE_TYPE_I;
            s->default_models_init(s);
            for (block=0; block<s->mb_height*s->mb_width; block++)
                s->macroblocks[block].type = VP56_MB_INTRA;
        } else {
            p->pict_type = AV_PICTURE_TYPE_P;
            vp56_parse_mb_type_models(s);
            s->parse_vector_models(s);
            s->mb_type = VP56_MB_INTER_NOVEC_PF;
        }

        if (s->parse_coeff_models(s))
            goto next;

        memset(s->prev_dc, 0, sizeof(s->prev_dc));
        s->prev_dc[1][VP56_FRAME_CURRENT] = 128;
        s->prev_dc[2][VP56_FRAME_CURRENT] = 128;

        for (block=0; block < 4*s->mb_width+6; block++) {
            s->above_blocks[block].ref_frame = VP56_FRAME_NONE;
            s->above_blocks[block].dc_coeff = 0;
            s->above_blocks[block].not_null_dc = 0;
        }
        s->above_blocks[2*s->mb_width + 2].ref_frame = VP56_FRAME_CURRENT;
        s->above_blocks[3*s->mb_width + 4].ref_frame = VP56_FRAME_CURRENT;

        stride_y  = p->linesize[0];
        stride_uv = p->linesize[1];

        if (s->flip < 0)
            mb_offset = 7;

        /* main macroblocks loop */
        for (mb_row=0; mb_row<s->mb_height; mb_row++) {
            if (s->flip < 0)
                mb_row_flip = s->mb_height - mb_row - 1;
            else
                mb_row_flip = mb_row;

            for (block=0; block<4; block++) {
                s->left_block[block].ref_frame = VP56_FRAME_NONE;
                s->left_block[block].dc_coeff = 0;
                s->left_block[block].not_null_dc = 0;
            }
            memset(s->coeff_ctx, 0, sizeof(s->coeff_ctx));
            memset(s->coeff_ctx_last, 24, sizeof(s->coeff_ctx_last));

            s->above_block_idx[0] = 1;
            s->above_block_idx[1] = 2;
            s->above_block_idx[2] = 1;
            s->above_block_idx[3] = 2;
            s->above_block_idx[4] = 2*s->mb_width + 2 + 1;
            s->above_block_idx[5] = 3*s->mb_width + 4 + 1;

            s->block_offset[s->frbi] = (mb_row_flip*16 + mb_offset) * stride_y;
            s->block_offset[s->srbi] = s->block_offset[s->frbi] + 8*stride_y;
            s->block_offset[1] = s->block_offset[0] + 8;
            s->block_offset[3] = s->block_offset[2] + 8;
            s->block_offset[4] = (mb_row_flip*8 + mb_offset) * stride_uv;
            s->block_offset[5] = s->block_offset[4];

            for (mb_col=0; mb_col<s->mb_width; mb_col++) {
                vp56_decode_mb(s, mb_row, mb_col, is_alpha);

                for (y=0; y<4; y++) {
                    s->above_block_idx[y] += 2;
                    s->block_offset[y] += 16;
                }

                for (uv=4; uv<6; uv++) {
                    s->above_block_idx[uv] += 1;
                    s->block_offset[uv] += 8;
                }
            }
        }

    next:
        if (p->key_frame || golden_frame) {
            if (s->framep[VP56_FRAME_GOLDEN]->data[0] && s->framep[VP56_FRAME_GOLDEN] != p &&
                s->framep[VP56_FRAME_GOLDEN] != s->framep[VP56_FRAME_GOLDEN2])
                avctx->release_buffer(avctx, s->framep[VP56_FRAME_GOLDEN]);
            s->framep[VP56_FRAME_GOLDEN] = p;
        }

        if (s->has_alpha) {
            FFSWAP(AVFrame *, s->framep[VP56_FRAME_GOLDEN],
                              s->framep[VP56_FRAME_GOLDEN2]);
            buf += alpha_offset;
            remaining_buf_size -= alpha_offset;
        }
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    if (s->framep[VP56_FRAME_PREVIOUS] == s->framep[VP56_FRAME_GOLDEN] ||
        s->framep[VP56_FRAME_PREVIOUS] == s->framep[VP56_FRAME_GOLDEN2]) {
        if (s->framep[VP56_FRAME_UNUSED] != s->framep[VP56_FRAME_GOLDEN] &&
            s->framep[VP56_FRAME_UNUSED] != s->framep[VP56_FRAME_GOLDEN2])
            FFSWAP(AVFrame *, s->framep[VP56_FRAME_PREVIOUS],
                              s->framep[VP56_FRAME_UNUSED]);
        else
            FFSWAP(AVFrame *, s->framep[VP56_FRAME_PREVIOUS],
                              s->framep[VP56_FRAME_UNUSED2]);
    } else if (s->framep[VP56_FRAME_PREVIOUS]->data[0])
        avctx->release_buffer(avctx, s->framep[VP56_FRAME_PREVIOUS]);
    FFSWAP(AVFrame *, s->framep[VP56_FRAME_CURRENT],
                      s->framep[VP56_FRAME_PREVIOUS]);

    p->qstride = 0;
    p->qscale_table = s->qscale_table;
    p->qscale_type = FF_QSCALE_TYPE_VP56;
    *(AVFrame*)data = *p;
    *data_size = sizeof(AVFrame);

    return avpkt->size;
}
