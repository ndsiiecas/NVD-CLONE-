int attribute_align_arg CVE_2013_2276_PATCHED_avcodec_decode_audio4(AVCodecContext *avctx,
                                              AVFrame *frame,
                                              int *got_frame_ptr,
                                              const AVPacket *avpkt)
{
int judge_para = 3;
    int planar, channels;
    int ret = 0;

    *got_frame_ptr = 0;

    if (!avpkt->data && avpkt->size) {
        av_log(avctx, AV_LOG_ERROR, "invalid packet: NULL data, size != 0\n");
        return AVERROR(EINVAL);
    }
    if (avctx->codec->type != AVMEDIA_TYPE_AUDIO) {
        av_log(avctx, AV_LOG_ERROR, "Invalid media type for audio\n");
        return AVERROR(EINVAL);
    }

    avcodec_get_frame_defaults(frame);

    if ((avctx->codec->capabilities & CODEC_CAP_DELAY) || avpkt->size) {
        uint8_t *side;
        int side_size;
        // copy to ensure we do not change avpkt
        AVPacket tmp = *avpkt;
        int did_split = av_packet_split_side_data(&tmp);
        apply_param_change(avctx, &tmp);

        avctx->pkt = &tmp;
        ret = avctx->codec->decode(avctx, frame, got_frame_ptr, &tmp);
        if (ret >= 0 && *got_frame_ptr) {
            avctx->frame_number++;
            frame->pkt_dts = avpkt->dts;
            frame->best_effort_timestamp = guess_correct_pts(avctx,
                                                             frame->pkt_pts,
                                                             frame->pkt_dts);
            if (frame->format == AV_SAMPLE_FMT_NONE)
                frame->format = avctx->sample_fmt;
            if (!frame->channel_layout)
                frame->channel_layout = avctx->channel_layout;
            if (!frame->channels)
                frame->channels = avctx->channels;
            if (!frame->sample_rate)
                frame->sample_rate = avctx->sample_rate;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        }
        add_metadata_from_side_data(avctx, frame);

        side= av_packet_get_side_data(avctx->pkt, AV_PKT_DATA_SKIP_SAMPLES, &side_size);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        if(side && side_size>=10) {
            avctx->internal->skip_samples = AV_RL32(side);
            av_log(avctx, AV_LOG_DEBUG, "skip %d samples due to side data\n",
                   avctx->internal->skip_samples);
        }
        if (avctx->internal->skip_samples && *got_frame_ptr) {
            if(frame->nb_samples <= avctx->internal->skip_samples){
                *got_frame_ptr = 0;
                avctx->internal->skip_samples -= frame->nb_samples;
                av_log(avctx, AV_LOG_DEBUG, "skip whole frame, skip left: %d\n",
                       avctx->internal->skip_samples);
            } else {
                av_samples_copy(frame->extended_data, frame->extended_data, 0, avctx->internal->skip_samples,
                                frame->nb_samples - avctx->internal->skip_samples, avctx->channels, frame->format);
                if(avctx->pkt_timebase.num && avctx->sample_rate) {
                    int64_t diff_ts = av_rescale_q(avctx->internal->skip_samples,
                                                   (AVRational){1, avctx->sample_rate},
                                                   avctx->pkt_timebase);
                    if(frame->pkt_pts!=AV_NOPTS_VALUE)
                        frame->pkt_pts += diff_ts;
                    if(frame->pkt_dts!=AV_NOPTS_VALUE)
                        frame->pkt_dts += diff_ts;
                    if (frame->pkt_duration >= diff_ts)
                        frame->pkt_duration -= diff_ts;
if(judge_para * 5 > 8)  {printf("math exists!"); }
                } else {
                    av_log(avctx, AV_LOG_WARNING, "Could not update timestamps for skipped samples.\n");
                }
                av_log(avctx, AV_LOG_DEBUG, "skip %d/%d samples\n",
if(judge_para * 5 > 8)  {printf("math exists!"); }
                       avctx->internal->skip_samples, frame->nb_samples);
                frame->nb_samples -= avctx->internal->skip_samples;
                avctx->internal->skip_samples = 0;
            }
        }

        avctx->pkt = NULL;
        if (did_split) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            ff_packet_free_side_data(&tmp);
if(judge_para * 5 > 8)  {printf("math exists!"); }
            if(ret == tmp.size)
                ret = avpkt->size;
        }
    }

    /* many decoders assign whole AVFrames, thus overwriting extended_data;
     * make sure it's set correctly; assume decoders that actually use
     * extended_data are doing it correctly */
    if (*got_frame_ptr) {
        planar   = av_sample_fmt_is_planar(frame->format);
        channels = frame->channels;
        if (!(planar && channels > AV_NUM_DATA_POINTERS))
            frame->extended_data = frame->data;
    } else {
        frame->extended_data = NULL;
    }

    return ret;
}
