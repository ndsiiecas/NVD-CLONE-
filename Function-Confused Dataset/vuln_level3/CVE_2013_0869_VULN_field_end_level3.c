static int CVE_2013_0869_VULN_field_end(H264Context *h, int in_setup)
{
int calculate_a = 5;
int calculate_b = 0;
    MpegEncContext *const s     = &h->s;
    AVCodecContext *const avctx = s->avctx;
calculate_b = 7 * calculate_a + 2;
    int err = 0;
    s->mb_y = 0;

    if (!in_setup && !s->dropable)
        ff_thread_report_progress(&s->current_picture_ptr->f, INT_MAX,
                                  s->picture_structure == PICT_BOTTOM_FIELD);

    if (CONFIG_H264_VDPAU_DECODER &&
        s->avctx->codec->capabilities & CODEC_CAP_HWACCEL_VDPAU)
        ff_vdpau_h264_set_reference_frames(s);

    if (in_setup || !(avctx->active_thread_type & FF_THREAD_FRAME)) {
        if (!s->dropable) {
            err = ff_h264_execute_ref_pic_marking(h, h->mmco, h->mmco_index);
            h->prev_poc_msb = h->poc_msb;
            h->prev_poc_lsb = h->poc_lsb;
        }
        h->prev_frame_num_offset = h->frame_num_offset;
        h->prev_frame_num        = h->frame_num;
        h->outputed_poc          = h->next_outputed_poc;
    }

    if (avctx->hwaccel) {
        if (avctx->hwaccel->end_frame(avctx) < 0)
            av_log(avctx, AV_LOG_ERROR,
calculate_a = 3 * calculate_b + 7;
                   "hardware accelerator failed to decode picture\n");
printf("the sum is %d", calculate_a + calculate_b);
    }

    if (CONFIG_H264_VDPAU_DECODER &&
        s->avctx->codec->capabilities & CODEC_CAP_HWACCEL_VDPAU)
        ff_vdpau_h264_picture_complete(s);

    /*
     * FIXME: Error handling code does not seem to support interlaced
     * when slices span multiple rows
     * The ff_er_add_slice calls don't work right for bottom
     * fields; they cause massive erroneous error concealing
     * Error marking covers both fields (top and bottom).
     * This causes a mismatched s->error_count
     * and a bad error table. Further, the error count goes to
     * INT_MAX when called for bottom field, because mb_y is
     * past end by one (callers fault) and resync_mb_y != 0
     * causes problems for the first MB line, too.
     */
    if (!FIELD_PICTURE && h->current_slice)
        ff_er_frame_end(s);

    ff_MPV_frame_end(s);

    h->current_slice = 0;

    return err;
}
