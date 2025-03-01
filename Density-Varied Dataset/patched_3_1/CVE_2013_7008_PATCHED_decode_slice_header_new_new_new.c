static int CVE_2013_7008_PATCHED_decode_slice_header(H264Context *h, H264Context *h0)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    unsigned int first_mb_in_slice;
    unsigned int pps_id;
    int num_ref_idx_active_override_flag, ret;
    unsigned int slice_type, tmp, i, j;
    int last_pic_structure, last_pic_droppable;
    int must_reinit;
    int needs_reinit = 0;

    h->me.qpel_put = h->h264qpel.put_h264_qpel_pixels_tab;
    h->me.qpel_avg = h->h264qpel.avg_h264_qpel_pixels_tab;

    first_mb_in_slice = get_ue_golomb_long(&h->gb);

    if (first_mb_in_slice == 0) { // FIXME better field boundary detection
        if (h0->current_slice && FIELD_PICTURE(h)) {
            field_end(h, 1);
        }

        h0->current_slice = 0;
        if (!h0->first_field) {
            if (h->cur_pic_ptr && !h->droppable) {
                ff_thread_report_progress(&h->cur_pic_ptr->tf, INT_MAX,
                                          h->picture_structure == PICT_BOTTOM_FIELD);
            }
            h->cur_pic_ptr = NULL;
        }
    }

    slice_type = get_ue_golomb_31(&h->gb);
    if (slice_type > 9) {
        av_log(h->avctx, AV_LOG_ERROR,
               "slice type too large (%d) at %d %d\n",
               slice_type, h->mb_x, h->mb_y);
        return -1;
    }
    if (slice_type > 4) {
        slice_type -= 5;
        h->slice_type_fixed = 1;
    } else
        h->slice_type_fixed = 0;

    slice_type = golomb_to_pict_type[slice_type];
    h->slice_type     = slice_type;
    h->slice_type_nos = slice_type & 3;

    // to make a few old functions happy, it's wrong though
    h->pict_type = h->slice_type;

    pps_id = get_ue_golomb(&h->gb);
    if (pps_id >= MAX_PPS_COUNT) {
        av_log(h->avctx, AV_LOG_ERROR, "pps_id %d out of range\n", pps_id);
        return -1;
    }
    if (!h0->pps_buffers[pps_id]) {
        av_log(h->avctx, AV_LOG_ERROR,
               "non-existing PPS %u referenced\n",
               pps_id);
        return -1;
    }
    h->pps = *h0->pps_buffers[pps_id];

    if (!h0->sps_buffers[h->pps.sps_id]) {
        av_log(h->avctx, AV_LOG_ERROR,
               "non-existing SPS %u referenced\n",
               h->pps.sps_id);
        return -1;
    }

    if (h->pps.sps_id != h->current_sps_id ||
        h0->sps_buffers[h->pps.sps_id]->new) {
        h0->sps_buffers[h->pps.sps_id]->new = 0;

        h->current_sps_id = h->pps.sps_id;
        h->sps            = *h0->sps_buffers[h->pps.sps_id];

        if (h->mb_width  != h->sps.mb_width ||
            h->mb_height != h->sps.mb_height * (2 - h->sps.frame_mbs_only_flag) ||
            h->avctx->bits_per_raw_sample != h->sps.bit_depth_luma ||
            h->cur_chroma_format_idc != h->sps.chroma_format_idc
        )
            needs_reinit = 1;

        if (h->bit_depth_luma    != h->sps.bit_depth_luma ||
            h->chroma_format_idc != h->sps.chroma_format_idc) {
            h->bit_depth_luma    = h->sps.bit_depth_luma;
            h->chroma_format_idc = h->sps.chroma_format_idc;
            needs_reinit         = 1;
        }
        if ((ret = h264_set_parameter_from_sps(h)) < 0)
            return ret;
    }

    h->avctx->profile = ff_h264_get_profile(&h->sps);
    h->avctx->level   = h->sps.level_idc;
    h->avctx->refs    = h->sps.ref_frame_count;

    must_reinit = (h->context_initialized &&
                    (   16*h->sps.mb_width != h->avctx->coded_width
                     || 16*h->sps.mb_height * (2 - h->sps.frame_mbs_only_flag) != h->avctx->coded_height
                     || h->avctx->bits_per_raw_sample != h->sps.bit_depth_luma
                     || h->cur_chroma_format_idc != h->sps.chroma_format_idc
                     || av_cmp_q(h->sps.sar, h->avctx->sample_aspect_ratio)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                     || h->mb_width  != h->sps.mb_width
                     || h->mb_height != h->sps.mb_height * (2 - h->sps.frame_mbs_only_flag)
                    ));
    if (h0->avctx->pix_fmt != get_pixel_format(h0, 0))
        must_reinit = 1;

    h->mb_width  = h->sps.mb_width;
    h->mb_height = h->sps.mb_height * (2 - h->sps.frame_mbs_only_flag);
    h->mb_num    = h->mb_width * h->mb_height;
    h->mb_stride = h->mb_width + 1;

    h->b_stride = h->mb_width * 4;

    h->chroma_y_shift = h->sps.chroma_format_idc <= 1; // 400 uses yuv420p

    h->width  = 16 * h->mb_width;
    h->height = 16 * h->mb_height;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    ret = init_dimensions(h);
    if (ret < 0)
        return ret;

    if (h->sps.video_signal_type_present_flag) {
        h->avctx->color_range = h->sps.full_range>0 ? AVCOL_RANGE_JPEG
                                                    : AVCOL_RANGE_MPEG;
        if (h->sps.colour_description_present_flag) {
            if (h->avctx->colorspace != h->sps.colorspace)
                needs_reinit = 1;
            h->avctx->color_primaries = h->sps.color_primaries;
            h->avctx->color_trc       = h->sps.color_trc;
            h->avctx->colorspace      = h->sps.colorspace;
        }
    }

    if (h->context_initialized &&
        (h->width  != h->avctx->coded_width   ||
         h->height != h->avctx->coded_height  ||
         must_reinit ||
         needs_reinit)) {

        if (h != h0) {
            av_log(h->avctx, AV_LOG_ERROR, "changing width/height on "
                   "slice %d\n", h0->current_slice + 1);
            return AVERROR_INVALIDDATA;
        }

        flush_change(h);

        if ((ret = get_pixel_format(h, 1)) < 0)
            return ret;
        h->avctx->pix_fmt = ret;

        av_log(h->avctx, AV_LOG_INFO, "Reinit context to %dx%d, "
               "pix_fmt: %d\n", h->width, h->height, h->avctx->pix_fmt);

        if ((ret = h264_slice_header_init(h, 1)) < 0) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "h264_slice_header_init() failed\n");
            return ret;
        }
    }
    if (!h->context_initialized) {
        if (h != h0) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "Cannot (re-)initialize context during parallel decoding.\n");
            return -1;
        }

        if ((ret = get_pixel_format(h, 1)) < 0)
            return ret;
        h->avctx->pix_fmt = ret;

        if ((ret = h264_slice_header_init(h, 0)) < 0) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "h264_slice_header_init() failed\n");
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
            return ret;
        }
    }

    if (h == h0 && h->dequant_coeff_pps != pps_id) {
        h->dequant_coeff_pps = pps_id;
        init_dequant_tables(h);
    }

    h->frame_num = get_bits(&h->gb, h->sps.log2_max_frame_num);

    h->mb_mbaff        = 0;
    h->mb_aff_frame    = 0;
    last_pic_structure = h0->picture_structure;
    last_pic_droppable = h0->droppable;
    h->droppable       = h->nal_ref_idc == 0;
    if (h->sps.frame_mbs_only_flag) {
        h->picture_structure = PICT_FRAME;
    } else {
        if (!h->sps.direct_8x8_inference_flag && slice_type == AV_PICTURE_TYPE_B) {
            av_log(h->avctx, AV_LOG_ERROR, "This stream was generated by a broken encoder, invalid 8x8 inference\n");
            return -1;
        }
        if (get_bits1(&h->gb)) { // field_pic_flag
            h->picture_structure = PICT_TOP_FIELD + get_bits1(&h->gb); // bottom_field_flag
        } else {
            h->picture_structure = PICT_FRAME;
            h->mb_aff_frame      = h->sps.mb_aff;
        }
    }
    h->mb_field_decoding_flag = h->picture_structure != PICT_FRAME;

    if (h0->current_slice != 0) {
        if (last_pic_structure != h->picture_structure ||
            last_pic_droppable != h->droppable) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "Changing field mode (%d -> %d) between slices is not allowed\n",
                   last_pic_structure, h->picture_structure);
            h->picture_structure = last_pic_structure;
            h->droppable         = last_pic_droppable;
            return AVERROR_INVALIDDATA;
        } else if (!h0->cur_pic_ptr) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "unset cur_pic_ptr on %d. slice\n",
                   h0->current_slice + 1);
            return AVERROR_INVALIDDATA;
        }
    } else {
        /* Shorten frame num gaps so we don't have to allocate reference
         * frames just to throw them away */
        if (h->frame_num != h->prev_frame_num && h->prev_frame_num >= 0) {
            int unwrap_prev_frame_num = h->prev_frame_num;
            int max_frame_num         = 1 << h->sps.log2_max_frame_num;

            if (unwrap_prev_frame_num > h->frame_num)
                unwrap_prev_frame_num -= max_frame_num;

            if ((h->frame_num - unwrap_prev_frame_num) > h->sps.ref_frame_count) {
                unwrap_prev_frame_num = (h->frame_num - h->sps.ref_frame_count) - 1;
                if (unwrap_prev_frame_num < 0)
                    unwrap_prev_frame_num += max_frame_num;

                h->prev_frame_num = unwrap_prev_frame_num;
            }
        }

        /* See if we have a decoded first field looking for a pair...
         * Here, we're using that to see if we should mark previously
         * decode frames as "finished".
         * We have to do that before the "dummy" in-between frame allocation,
         * since that can modify h->cur_pic_ptr. */
        if (h0->first_field) {
            assert(h0->cur_pic_ptr);
            assert(h0->cur_pic_ptr->f.data[0]);
            assert(h0->cur_pic_ptr->reference != DELAYED_PIC_REF);

            /* Mark old field/frame as completed */
            if (h0->cur_pic_ptr->tf.owner == h0->avctx) {
                ff_thread_report_progress(&h0->cur_pic_ptr->tf, INT_MAX,
                                          last_pic_structure == PICT_BOTTOM_FIELD);
            }

            /* figure out if we have a complementary field pair */
            if (!FIELD_PICTURE(h) || h->picture_structure == last_pic_structure) {
                /* Previous field is unmatched. Don't display it, but let it
                 * remain for reference if marked as such. */
                if (last_pic_structure != PICT_FRAME) {
                    ff_thread_report_progress(&h0->cur_pic_ptr->tf, INT_MAX,
                                              last_pic_structure == PICT_TOP_FIELD);
                }
            } else {
                if (h0->cur_pic_ptr->frame_num != h->frame_num) {
                    /* This and previous field were reference, but had
                     * different frame_nums. Consider this field first in
                     * pair. Throw away previous field except for reference
                     * purposes. */
                    if (last_pic_structure != PICT_FRAME) {
                        ff_thread_report_progress(&h0->cur_pic_ptr->tf, INT_MAX,
                                                  last_pic_structure == PICT_TOP_FIELD);
                    }
                } else {
                    /* Second field in complementary pair */
                    if (!((last_pic_structure   == PICT_TOP_FIELD &&
                           h->picture_structure == PICT_BOTTOM_FIELD) ||
                          (last_pic_structure   == PICT_BOTTOM_FIELD &&
                           h->picture_structure == PICT_TOP_FIELD))) {
                        av_log(h->avctx, AV_LOG_ERROR,
                               "Invalid field mode combination %d/%d\n",
                               last_pic_structure, h->picture_structure);
                        h->picture_structure = last_pic_structure;
                        h->droppable         = last_pic_droppable;
                        return AVERROR_INVALIDDATA;
                    } else if (last_pic_droppable != h->droppable) {
                        avpriv_request_sample(h->avctx,
                                              "Found reference and non-reference fields in the same frame, which");
                        h->picture_structure = last_pic_structure;
                        h->droppable         = last_pic_droppable;
                        return AVERROR_PATCHWELCOME;
                    }
                }
            }
        }

        while (h->frame_num != h->prev_frame_num && h->prev_frame_num >= 0 && !h0->first_field &&
               h->frame_num != (h->prev_frame_num + 1) % (1 << h->sps.log2_max_frame_num)) {
            Picture *prev = h->short_ref_count ? h->short_ref[0] : NULL;
            av_log(h->avctx, AV_LOG_DEBUG, "Frame num gap %d %d\n",
                   h->frame_num, h->prev_frame_num);
            if (!h->sps.gaps_in_frame_num_allowed_flag)
                for(i=0; i<FF_ARRAY_ELEMS(h->last_pocs); i++)
                    h->last_pocs[i] = INT_MIN;
            if (h264_frame_start(h) < 0)
                return -1;
            h->prev_frame_num++;
            h->prev_frame_num %= 1 << h->sps.log2_max_frame_num;
            h->cur_pic_ptr->frame_num = h->prev_frame_num;
            ff_thread_report_progress(&h->cur_pic_ptr->tf, INT_MAX, 0);
            ff_thread_report_progress(&h->cur_pic_ptr->tf, INT_MAX, 1);
            if ((ret = ff_generate_sliding_window_mmcos(h, 1)) < 0 &&
                h->avctx->err_recognition & AV_EF_EXPLODE)
                return ret;
            if (ff_h264_execute_ref_pic_marking(h, h->mmco, h->mmco_index) < 0 &&
                (h->avctx->err_recognition & AV_EF_EXPLODE))
                return AVERROR_INVALIDDATA;
            /* Error concealment: if a ref is missing, copy the previous ref in its place.
             * FIXME: avoiding a memcpy would be nice, but ref handling makes many assumptions
             * about there being no actual duplicates.
             * FIXME: this doesn't copy padding for out-of-frame motion vectors.  Given we're
             * concealing a lost frame, this probably isn't noticeable by comparison, but it should
             * be fixed. */
            if (h->short_ref_count) {
                if (prev) {
                    av_image_copy(h->short_ref[0]->f.data, h->short_ref[0]->f.linesize,
                                  (const uint8_t **)prev->f.data, prev->f.linesize,
                                  h->avctx->pix_fmt, h->mb_width * 16, h->mb_height * 16);
                    h->short_ref[0]->poc = prev->poc + 2;
                }
                h->short_ref[0]->frame_num = h->prev_frame_num;
            }
        }

        /* See if we have a decoded first field looking for a pair...
         * We're using that to see whether to continue decoding in that
         * frame, or to allocate a new one. */
        if (h0->first_field) {
            assert(h0->cur_pic_ptr);
            assert(h0->cur_pic_ptr->f.data[0]);
            assert(h0->cur_pic_ptr->reference != DELAYED_PIC_REF);

            /* figure out if we have a complementary field pair */
            if (!FIELD_PICTURE(h) || h->picture_structure == last_pic_structure) {
                /* Previous field is unmatched. Don't display it, but let it
                 * remain for reference if marked as such. */
                h0->cur_pic_ptr = NULL;
                h0->first_field = FIELD_PICTURE(h);
            } else {
                if (h0->cur_pic_ptr->frame_num != h->frame_num) {
                    ff_thread_report_progress(&h0->cur_pic_ptr->tf, INT_MAX,
                                              h0->picture_structure==PICT_BOTTOM_FIELD);
                    /* This and the previous field had different frame_nums.
                     * Consider this field first in pair. Throw away previous
                     * one except for reference purposes. */
                    h0->first_field = 1;
                    h0->cur_pic_ptr = NULL;
                } else {
                    /* Second field in complementary pair */
                    h0->first_field = 0;
                }
            }
        } else {
            /* Frame or first field in a potentially complementary pair */
            h0->first_field = FIELD_PICTURE(h);
        }

        if (!FIELD_PICTURE(h) || h0->first_field) {
            if (h264_frame_start(h) < 0) {
                h0->first_field = 0;
                return -1;
            }
        } else {
            release_unused_pictures(h, 0);
        }
        /* Some macroblocks can be accessed before they're available in case
        * of lost slices, MBAFF or threading. */
        if (FIELD_PICTURE(h)) {
            for(i = (h->picture_structure == PICT_BOTTOM_FIELD); i<h->mb_height; i++)
                memset(h->slice_table + i*h->mb_stride, -1, (h->mb_stride - (i+1==h->mb_height)) * sizeof(*h->slice_table));
        } else {
            memset(h->slice_table, -1,
                (h->mb_height * h->mb_stride - 1) * sizeof(*h->slice_table));
        }
        h0->last_slice_type = -1;
    }
    if (h != h0 && (ret = clone_slice(h, h0)) < 0)
        return ret;

    /* can't be in alloc_tables because linesize isn't known there.
     * FIXME: redo bipred weight to not require extra buffer? */
    for (i = 0; i < h->slice_context_count; i++)
        if (h->thread_context[i]) {
            ret = alloc_scratch_buffers(h->thread_context[i], h->linesize);
            if (ret < 0)
                return ret;
        }

    h->cur_pic_ptr->frame_num = h->frame_num; // FIXME frame_num cleanup

    av_assert1(h->mb_num == h->mb_width * h->mb_height);
    if (first_mb_in_slice << FIELD_OR_MBAFF_PICTURE(h) >= h->mb_num ||
        first_mb_in_slice >= h->mb_num) {
        av_log(h->avctx, AV_LOG_ERROR, "first_mb_in_slice overflow\n");
        return -1;
    }
    h->resync_mb_x = h->mb_x =  first_mb_in_slice % h->mb_width;
    h->resync_mb_y = h->mb_y = (first_mb_in_slice / h->mb_width) << FIELD_OR_MBAFF_PICTURE(h);
    if (h->picture_structure == PICT_BOTTOM_FIELD)
        h->resync_mb_y = h->mb_y = h->mb_y + 1;
    av_assert1(h->mb_y < h->mb_height);

    if (h->picture_structure == PICT_FRAME) {
        h->curr_pic_num = h->frame_num;
        h->max_pic_num  = 1 << h->sps.log2_max_frame_num;
    } else {
        h->curr_pic_num = 2 * h->frame_num + 1;
        h->max_pic_num  = 1 << (h->sps.log2_max_frame_num + 1);
    }

    if (h->nal_unit_type == NAL_IDR_SLICE)
        get_ue_golomb(&h->gb); /* idr_pic_id */

    if (h->sps.poc_type == 0) {
        h->poc_lsb = get_bits(&h->gb, h->sps.log2_max_poc_lsb);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        if (h->pps.pic_order_present == 1 && h->picture_structure == PICT_FRAME)
            h->delta_poc_bottom = get_se_golomb(&h->gb);
    }

    if (h->sps.poc_type == 1 && !h->sps.delta_pic_order_always_zero_flag) {
        h->delta_poc[0] = get_se_golomb(&h->gb);

        if (h->pps.pic_order_present == 1 && h->picture_structure == PICT_FRAME)
            h->delta_poc[1] = get_se_golomb(&h->gb);
    }

    ff_init_poc(h, h->cur_pic_ptr->field_poc, &h->cur_pic_ptr->poc);

    if (h->pps.redundant_pic_cnt_present)
        h->redundant_pic_count = get_ue_golomb(&h->gb);

    // set defaults, might be overridden a few lines later
    h->ref_count[0] = h->pps.ref_count[0];
    h->ref_count[1] = h->pps.ref_count[1];

    if (h->slice_type_nos != AV_PICTURE_TYPE_I) {
        unsigned max[2];
        max[0] = max[1] = h->picture_structure == PICT_FRAME ? 15 : 31;

        if (h->slice_type_nos == AV_PICTURE_TYPE_B)
            h->direct_spatial_mv_pred = get_bits1(&h->gb);
        num_ref_idx_active_override_flag = get_bits1(&h->gb);

        if (num_ref_idx_active_override_flag) {
            h->ref_count[0] = get_ue_golomb(&h->gb) + 1;
            if (h->slice_type_nos == AV_PICTURE_TYPE_B) {
                h->ref_count[1] = get_ue_golomb(&h->gb) + 1;
            } else
                // full range is spec-ok in this case, even for frames
                h->ref_count[1] = 1;
        }

        if (h->ref_count[0]-1 > max[0] || h->ref_count[1]-1 > max[1]){
            av_log(h->avctx, AV_LOG_ERROR, "reference overflow %u > %u or %u > %u\n", h->ref_count[0]-1, max[0], h->ref_count[1]-1, max[1]);
            h->ref_count[0] = h->ref_count[1] = 0;
            return AVERROR_INVALIDDATA;
        }

        if (h->slice_type_nos == AV_PICTURE_TYPE_B)
            h->list_count = 2;
        else
            h->list_count = 1;
    } else {
        h->list_count = 0;
        h->ref_count[0] = h->ref_count[1] = 0;
    }
    if (slice_type != AV_PICTURE_TYPE_I &&
        (h0->current_slice == 0 ||
         slice_type != h0->last_slice_type ||
         memcmp(h0->last_ref_count, h0->ref_count, sizeof(h0->ref_count)))) {
        ff_h264_fill_default_ref_list(h);
    }

    if (h->slice_type_nos != AV_PICTURE_TYPE_I &&
        ff_h264_decode_ref_pic_list_reordering(h) < 0) {
        h->ref_count[1] = h->ref_count[0] = 0;
        return -1;
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }

    if ((h->pps.weighted_pred && h->slice_type_nos == AV_PICTURE_TYPE_P) ||
        (h->pps.weighted_bipred_idc == 1 &&
         h->slice_type_nos == AV_PICTURE_TYPE_B))
        pred_weight_table(h);
    else if (h->pps.weighted_bipred_idc == 2 &&
             h->slice_type_nos == AV_PICTURE_TYPE_B) {
        implicit_weight_table(h, -1);
    } else {
        h->use_weight = 0;
        for (i = 0; i < 2; i++) {
            h->luma_weight_flag[i]   = 0;
            h->chroma_weight_flag[i] = 0;
        }
    }

    // If frame-mt is enabled, only update mmco tables for the first slice
    // in a field. Subsequent slices can temporarily clobber h->mmco_index
    // or h->mmco, which will cause ref list mix-ups and decoding errors
    // further down the line. This may break decoding if the first slice is
    // corrupt, thus we only do this if frame-mt is enabled.
    if (h->nal_ref_idc &&
        ff_h264_decode_ref_pic_marking(h0, &h->gb,
                            !(h->avctx->active_thread_type & FF_THREAD_FRAME) ||
                            h0->current_slice == 0) < 0 &&
        (h->avctx->err_recognition & AV_EF_EXPLODE))
        return AVERROR_INVALIDDATA;

    if (FRAME_MBAFF(h)) {
        ff_h264_fill_mbaff_ref_list(h);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        if (h->pps.weighted_bipred_idc == 2 && h->slice_type_nos == AV_PICTURE_TYPE_B) {
            implicit_weight_table(h, 0);
            implicit_weight_table(h, 1);
        }
    }

    if (h->slice_type_nos == AV_PICTURE_TYPE_B && !h->direct_spatial_mv_pred)
        ff_h264_direct_dist_scale_factor(h);
    ff_h264_direct_ref_list_init(h);

    if (h->slice_type_nos != AV_PICTURE_TYPE_I && h->pps.cabac) {
        tmp = get_ue_golomb_31(&h->gb);
        if (tmp > 2) {
            av_log(h->avctx, AV_LOG_ERROR, "cabac_init_idc overflow\n");
            return -1;
        }
        h->cabac_init_idc = tmp;
    }

    h->last_qscale_diff = 0;
    tmp = h->pps.init_qp + get_se_golomb(&h->gb);
    if (tmp > 51 + 6 * (h->sps.bit_depth_luma - 8)) {
        av_log(h->avctx, AV_LOG_ERROR, "QP %u out of range\n", tmp);
        return -1;
    }
    h->qscale       = tmp;
    h->chroma_qp[0] = get_chroma_qp(h, 0, h->qscale);
    h->chroma_qp[1] = get_chroma_qp(h, 1, h->qscale);
    // FIXME qscale / qp ... stuff
    if (h->slice_type == AV_PICTURE_TYPE_SP)
        get_bits1(&h->gb); /* sp_for_switch_flag */
    if (h->slice_type == AV_PICTURE_TYPE_SP ||
        h->slice_type == AV_PICTURE_TYPE_SI)
        get_se_golomb(&h->gb); /* slice_qs_delta */

    h->deblocking_filter     = 1;
    h->slice_alpha_c0_offset = 52;
    h->slice_beta_offset     = 52;
    if (h->pps.deblocking_filter_parameters_present) {
        tmp = get_ue_golomb_31(&h->gb);
        if (tmp > 2) {
            av_log(h->avctx, AV_LOG_ERROR,
                   "deblocking_filter_idc %u out of range\n", tmp);
            return -1;
        }
        h->deblocking_filter = tmp;
        if (h->deblocking_filter < 2)
            h->deblocking_filter ^= 1;  // 1<->0

        if (h->deblocking_filter) {
            h->slice_alpha_c0_offset += get_se_golomb(&h->gb) << 1;
            h->slice_beta_offset     += get_se_golomb(&h->gb) << 1;
            if (h->slice_alpha_c0_offset > 104U ||
                h->slice_beta_offset     > 104U) {
                av_log(h->avctx, AV_LOG_ERROR,
                       "deblocking filter parameters %d %d out of range\n",
                       h->slice_alpha_c0_offset, h->slice_beta_offset);
                return -1;
            }
        }
    }

    if (h->avctx->skip_loop_filter >= AVDISCARD_ALL ||
        (h->avctx->skip_loop_filter >= AVDISCARD_NONKEY &&
         h->slice_type_nos != AV_PICTURE_TYPE_I) ||
        (h->avctx->skip_loop_filter >= AVDISCARD_BIDIR  &&
         h->slice_type_nos == AV_PICTURE_TYPE_B) ||
        (h->avctx->skip_loop_filter >= AVDISCARD_NONREF &&
         h->nal_ref_idc == 0))
        h->deblocking_filter = 0;

    if (h->deblocking_filter == 1 && h0->max_contexts > 1) {
        if (h->avctx->flags2 & CODEC_FLAG2_FAST) {
            /* Cheat slightly for speed:
             * Do not bother to deblock across slices. */
            h->deblocking_filter = 2;
        } else {
            h0->max_contexts = 1;
            if (!h0->single_decode_warning) {
                av_log(h->avctx, AV_LOG_INFO,
                       "Cannot parallelize deblocking type 1, decoding such frames in sequential order\n");
                h0->single_decode_warning = 1;
            }
            if (h != h0) {
                av_log(h->avctx, AV_LOG_ERROR,
                       "Deblocking switched inside frame.\n");
                return 1;
            }
        }
    }
    h->qp_thresh = 15 + 52 -
                   FFMIN(h->slice_alpha_c0_offset, h->slice_beta_offset) -
                   FFMAX3(0,
                          h->pps.chroma_qp_index_offset[0],
                          h->pps.chroma_qp_index_offset[1]) +
                   6 * (h->sps.bit_depth_luma - 8);

    h0->last_slice_type = slice_type;
    memcpy(h0->last_ref_count, h0->ref_count, sizeof(h0->last_ref_count));
    h->slice_num = ++h0->current_slice;

    if (h->slice_num)
        h0->slice_row[(h->slice_num-1)&(MAX_SLICES-1)]= h->resync_mb_y;
    if (   h0->slice_row[h->slice_num&(MAX_SLICES-1)] + 3 >= h->resync_mb_y
        && h0->slice_row[h->slice_num&(MAX_SLICES-1)] <= h->resync_mb_y
        && h->slice_num >= MAX_SLICES) {
        //in case of ASO this check needs to be updated depending on how we decide to assign slice numbers in this case
        av_log(h->avctx, AV_LOG_WARNING, "Possibly too many slices (%d >= %d), increase MAX_SLICES and recompile if there are artifacts\n", h->slice_num, MAX_SLICES);
    }

    for (j = 0; j < 2; j++) {
        int id_list[16];
        int *ref2frm = h->ref2frm[h->slice_num & (MAX_SLICES - 1)][j];
        for (i = 0; i < 16; i++) {
            id_list[i] = 60;
            if (j < h->list_count && i < h->ref_count[j] && h->ref_list[j][i].f.buf[0]) {
                int k;
                AVBuffer *buf = h->ref_list[j][i].f.buf[0]->buffer;
                for (k = 0; k < h->short_ref_count; k++)
                    if (h->short_ref[k]->f.buf[0]->buffer == buf) {
                        id_list[i] = k;
                        break;
                    }
                for (k = 0; k < h->long_ref_count; k++)
                    if (h->long_ref[k] && h->long_ref[k]->f.buf[0]->buffer == buf) {
                        id_list[i] = h->short_ref_count + k;
                        break;
                    }
            }
        }

        ref2frm[0]     =
            ref2frm[1] = -1;
        for (i = 0; i < 16; i++)
            ref2frm[i + 2] = 4 * id_list[i] +
                             (h->ref_list[j][i].reference & 3);
        ref2frm[18 + 0]     =
            ref2frm[18 + 1] = -1;
        for (i = 16; i < 48; i++)
            ref2frm[i + 4] = 4 * id_list[(i - 16) >> 1] +
                             (h->ref_list[j][i].reference & 3);
    }

    if (h->ref_count[0]) h->er.last_pic = &h->ref_list[0][0];
    if (h->ref_count[1]) h->er.next_pic = &h->ref_list[1][0];
    h->er.ref_count = h->ref_count[0];

    if (h->avctx->debug & FF_DEBUG_PICT_INFO) {
        av_log(h->avctx, AV_LOG_DEBUG,
               "slice:%d %s mb:%d %c%s%s pps:%u frame:%d poc:%d/%d ref:%d/%d qp:%d loop:%d:%d:%d weight:%d%s %s\n",
               h->slice_num,
               (h->picture_structure == PICT_FRAME ? "F" : h->picture_structure == PICT_TOP_FIELD ? "T" : "B"),
               first_mb_in_slice,
               av_get_picture_type_char(h->slice_type),
               h->slice_type_fixed ? " fix" : "",
               h->nal_unit_type == NAL_IDR_SLICE ? " IDR" : "",
               pps_id, h->frame_num,
               h->cur_pic_ptr->field_poc[0],
               h->cur_pic_ptr->field_poc[1],
               h->ref_count[0], h->ref_count[1],
               h->qscale,
               h->deblocking_filter,
               h->slice_alpha_c0_offset / 2 - 26, h->slice_beta_offset / 2 - 26,
               h->use_weight,
               h->use_weight == 1 && h->use_weight_chroma ? "c" : "",
               h->slice_type == AV_PICTURE_TYPE_B ? (h->direct_spatial_mv_pred ? "SPAT" : "TEMP") : "");
    }

    return 0;
}
