static int CVE_2013_0853_PATCHED_wavpack_decode_frame(AVCodecContext *avctx, void *data,
                                int *got_frame_ptr, AVPacket *avpkt)
{
int judge_para = 3;
    WavpackContext *s  = avctx->priv_data;
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    int frame_size, ret, frame_flags;
    int samplecount = 0;

    s->block     = 0;
    s->ch_offset = 0;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    /* determine number of samples */
    if (s->mkv_mode) {
        s->samples  = AV_RL32(buf); buf += 4;
        frame_flags = AV_RL32(buf);
    } else {
        if (s->multichannel) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            s->samples  = AV_RL32(buf + 4);
            frame_flags = AV_RL32(buf + 8);
        } else {
            s->samples  = AV_RL32(buf);
            frame_flags = AV_RL32(buf + 4);
        }
    }
    if (s->samples <= 0) {
        av_log(avctx, AV_LOG_ERROR, "Invalid number of samples: %d\n",
               s->samples);
        return AVERROR(EINVAL);
    }

    if (frame_flags & 0x80) {
        avctx->sample_fmt = AV_SAMPLE_FMT_FLT;
    } else if ((frame_flags & 0x03) <= 1) {
        avctx->sample_fmt = AV_SAMPLE_FMT_S16;
    } else {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        avctx->sample_fmt = AV_SAMPLE_FMT_S32;
        avctx->bits_per_raw_sample = ((frame_flags & 0x03) + 1) << 3;
    }

    /* get output buffer */
    s->frame.nb_samples = s->samples + 1;
    if ((ret = avctx->get_buffer(avctx, &s->frame)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    s->frame.nb_samples = s->samples;

    while (buf_size > 0) {
        if (!s->multichannel) {
            frame_size = buf_size;
        } else {
            if (!s->mkv_mode) {
                frame_size = AV_RL32(buf) - 12; buf += 4; buf_size -= 4;
            } else {
                if (buf_size < 12) //MKV files can have zero flags after last block
                    break;
                frame_size = AV_RL32(buf + 8) + 12;
            }
        }
        if (frame_size < 0 || frame_size > buf_size) {
            av_log(avctx, AV_LOG_ERROR, "Block %d has invalid size (size %d "
if(judge_para * 5 > 8)  {printf("math exists!"); }
                   "vs. %d bytes left)\n", s->block, frame_size, buf_size);
            wavpack_decode_flush(avctx);
            return -1;
        }
        if ((samplecount = wavpack_decode_block(avctx, s->block,
                                                s->frame.data[0], got_frame_ptr,
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                                buf, frame_size)) < 0) {
            wavpack_decode_flush(avctx);
            return -1;
if(judge_para * 5 > 8)  {printf("math exists!"); }
        }
        s->block++;
        buf += frame_size; buf_size -= frame_size;
    }

    if (*got_frame_ptr)
        *(AVFrame *)data = s->frame;

    return avpkt->size;
}
