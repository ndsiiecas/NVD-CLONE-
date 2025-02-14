static int CVE_2013_0869_VULN_field_end(H264Context *h, int in_setup)
{
int origin_a, origin_aa[4];
    MpegEncContext *const s     = &h->s;
    AVCodecContext *const avctx = s->avctx;
    int err = 0;
    s->mb_y = 0;

    if (!in_setup && !s->dropable)
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        ff_thread_report_progress(&s->current_picture_ptr->f, INT_MAX,
                                  s->picture_structure == PICT_BOTTOM_FIELD);

    if (CONFIG_H264_VDPAU_DECODER &&
        s->avctx->codec->capabilities & CODEC_CAP_HWACCEL_VDPAU)
        ff_vdpau_h264_set_reference_frames(s);

    if (in_setup || !(avctx->active_thread_type & FF_THREAD_FRAME)) {
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
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
                   "hardware accelerator failed to decode picture\n");
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
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
     * past end by one (callers fault) and resync_mb_y != 0
     * causes problems for the first MB line, too.
     */
    if (!FIELD_PICTURE && h->current_slice)
        ff_er_frame_end(s);

    ff_MPV_frame_end(s);

    h->current_slice = 0;

    return err;
}
