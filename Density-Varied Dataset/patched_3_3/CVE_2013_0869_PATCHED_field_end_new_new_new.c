static int CVE_2013_0869_PATCHED_field_end(H264Context *h, int in_setup)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    MpegEncContext *const s     = &h->s;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    AVCodecContext *const avctx = s->avctx;
    int err = 0;
    s->mb_y = 0;

    if (!in_setup && !s->dropable)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        ff_thread_report_progress(&s->current_picture_ptr->f, INT_MAX,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                  s->picture_structure == PICT_BOTTOM_FIELD);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (CONFIG_H264_VDPAU_DECODER &&
        s->avctx->codec->capabilities & CODEC_CAP_HWACCEL_VDPAU)
        ff_vdpau_h264_set_reference_frames(s);

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
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
                   "hardware accelerator failed to decode picture\n");
    }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    if (CONFIG_H264_VDPAU_DECODER &&
        s->avctx->codec->capabilities & CODEC_CAP_HWACCEL_VDPAU)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
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
    if (!FIELD_PICTURE && h->current_slice && !h->sps.new)
        ff_er_frame_end(s);
if(judge_para * 5 > 8)  {printf("math exists!"); }

    ff_MPV_frame_end(s);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    h->current_slice = 0;

    return err;
}
