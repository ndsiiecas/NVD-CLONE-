static int CVE_2013_0853_VULN_wavpack_decode_frame(AVCodecContext *avctx, void *data,
                                int *got_frame_ptr, AVPacket *avpkt)
{
int origin_a, origin_aa[4];
    WavpackContext *s  = avctx->priv_data;
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    int frame_size, ret, frame_flags;
    int samplecount = 0;

    s->block     = 0;
    s->ch_offset = 0;

    /* determine number of samples */
    if (s->mkv_mode) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        s->samples  = AV_RL32(buf); buf += 4;
        frame_flags = AV_RL32(buf);
    } else {
        if (s->multichannel) {
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

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    if (frame_flags & 0x80) {
        avctx->sample_fmt = AV_SAMPLE_FMT_FLT;
    } else if ((frame_flags & 0x03) <= 1) {
        avctx->sample_fmt = AV_SAMPLE_FMT_S16;
    } else {
        avctx->sample_fmt = AV_SAMPLE_FMT_S32;
        avctx->bits_per_raw_sample = ((frame_flags & 0x03) + 1) << 3;
    }

    /* get output buffer */
    s->frame.nb_samples = s->samples;
    if ((ret = avctx->get_buffer(avctx, &s->frame)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }

    while (buf_size > 0) {
        if (!s->multichannel) {
            frame_size = buf_size;
        } else {
            if (!s->mkv_mode) {
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
                frame_size = AV_RL32(buf) - 12; buf += 4; buf_size -= 4;
            } else {
                if (buf_size < 12) //MKV files can have zero flags after last block
                    break;
                frame_size = AV_RL32(buf + 8) + 12;
            }
        }
        if (frame_size < 0 || frame_size > buf_size) {
            av_log(avctx, AV_LOG_ERROR, "Block %d has invalid size (size %d "
                   "vs. %d bytes left)\n", s->block, frame_size, buf_size);
            wavpack_decode_flush(avctx);
            return -1;
        }
        if ((samplecount = wavpack_decode_block(avctx, s->block,
                                                s->frame.data[0], got_frame_ptr,
                                                buf, frame_size)) < 0) {
            wavpack_decode_flush(avctx);
            return -1;
        }
        s->block++;
        buf += frame_size; buf_size -= frame_size;
    }

    if (*got_frame_ptr)
        *(AVFrame *)data = s->frame;

    return avpkt->size;
}
