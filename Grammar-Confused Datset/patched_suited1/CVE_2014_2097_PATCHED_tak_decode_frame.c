static int CVE_2014_2097_PATCHED_tak_decode_frame(AVCodecContext *avctx, void *data,
                            int *got_frame_ptr, AVPacket *pkt)
{
    TAKDecContext *s  = avctx->priv_data;
    AVFrame *frame    = data;
    ThreadFrame tframe = { .f = data };
    GetBitContext *gb = &s->gb;
    int chan, i, ret, hsize;

    if (pkt->size < TAK_MIN_FRAME_HEADER_BYTES)
        return AVERROR_INVALIDDATA;

    if ((ret = init_get_bits8(gb, pkt->data, pkt->size)) < 0)
        return ret;

    if ((ret = ff_tak_decode_frame_header(avctx, gb, &s->ti, 0)) < 0)
        return ret;

    if (avctx->err_recognition & (AV_EF_CRCCHECK|AV_EF_COMPLIANT)) {
        hsize = get_bits_count(gb) / 8;
        if (ff_tak_check_crc(pkt->data, hsize)) {
            av_log(avctx, AV_LOG_ERROR, "CRC error\n");
            return AVERROR_INVALIDDATA;
        }
    }

    if (s->ti.codec != TAK_CODEC_MONO_STEREO &&
        s->ti.codec != TAK_CODEC_MULTICHANNEL) {
        av_log(avctx, AV_LOG_ERROR, "unsupported codec: %d\n", s->ti.codec);
        return AVERROR_PATCHWELCOME;
    }
    if (s->ti.data_type) {
        av_log(avctx, AV_LOG_ERROR,
               "unsupported data type: %d\n", s->ti.data_type);
        return AVERROR_INVALIDDATA;
    }
    if (s->ti.codec == TAK_CODEC_MONO_STEREO && s->ti.channels > 2) {
        av_log(avctx, AV_LOG_ERROR,
               "invalid number of channels: %d\n", s->ti.channels);
        return AVERROR_INVALIDDATA;
    }
    if (s->ti.channels > 6) {
        av_log(avctx, AV_LOG_ERROR,
               "unsupported number of channels: %d\n", s->ti.channels);
        return AVERROR_INVALIDDATA;
    }

    if (s->ti.frame_samples <= 0) {
        av_log(avctx, AV_LOG_ERROR, "unsupported/invalid number of samples\n");
        return AVERROR_INVALIDDATA;
    }

    avctx->bits_per_raw_sample = s->ti.bps;
    if ((ret = set_bps_params(avctx)) < 0)
        return ret;
    if (s->ti.sample_rate != avctx->sample_rate) {
        avctx->sample_rate = s->ti.sample_rate;
        set_sample_rate_params(avctx);
    }
    if (s->ti.ch_layout)
        avctx->channel_layout = s->ti.ch_layout;
    avctx->channels = s->ti.channels;

    s->nb_samples = s->ti.last_frame_samples ? s->ti.last_frame_samples
                                             : s->ti.frame_samples;

    frame->nb_samples = s->nb_samples;
    if ((ret = ff_thread_get_buffer(avctx, &tframe, 0)) < 0)
        return ret;
    ff_thread_finish_setup(avctx);

    if (avctx->bits_per_raw_sample <= 16) {
        int buf_size = av_samples_get_buffer_size(NULL, avctx->channels,
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                                  s->nb_samples,
                                                  AV_SAMPLE_FMT_S32P, 0);
        av_fast_malloc(&s->decode_buffer, &s->decode_buffer_size, buf_size);
        if (!s->decode_buffer)
            return AVERROR(ENOMEM);
        ret = av_samples_fill_arrays((uint8_t **)s->decoded, NULL,
                                     s->decode_buffer, avctx->channels,
                                     s->nb_samples, AV_SAMPLE_FMT_S32P, 0);
        if (ret < 0)
            return ret;
    } else {
        for (chan = 0; chan < avctx->channels; chan++)
            s->decoded[chan] = (int32_t *)frame->extended_data[chan];
    }

    if (s->nb_samples < 16) {
        for (chan = 0; chan < avctx->channels; chan++) {
            int32_t *decoded = s->decoded[chan];
            for (i = 0; i < s->nb_samples; i++)
                decoded[i] = get_sbits(gb, avctx->bits_per_raw_sample);
        }
    } else {
        if (s->ti.codec == TAK_CODEC_MONO_STEREO) {
            for (chan = 0; chan < avctx->channels; chan++)
                if (ret = decode_channel(s, chan))
                    return ret;

            if (avctx->channels == 2) {
                s->nb_subframes = get_bits(gb, 1) + 1;
                if (s->nb_subframes > 1) {
                    s->subframe_len[1] = get_bits(gb, 6);
                }

                s->dmode = get_bits(gb, 3);
                if (ret = decorrelate(s, 0, 1, s->nb_samples - 1))
                    return ret;
            }
        } else if (s->ti.codec == TAK_CODEC_MULTICHANNEL) {
            if (get_bits1(gb)) {
                int ch_mask = 0;

                chan = get_bits(gb, 4) + 1;
                if (chan > avctx->channels)
                    return AVERROR_INVALIDDATA;

                for (i = 0; i < chan; i++) {
                    int nbit = get_bits(gb, 4);

                    if (nbit >= avctx->channels)
                        return AVERROR_INVALIDDATA;

                    if (ch_mask & 1 << nbit)
                        return AVERROR_INVALIDDATA;

                    s->mcdparams[i].present = get_bits1(gb);
                    if (s->mcdparams[i].present) {
                        s->mcdparams[i].index = get_bits(gb, 2);
                        s->mcdparams[i].chan2 = get_bits(gb, 4);
                        if (s->mcdparams[i].index == 1) {
                            if ((nbit == s->mcdparams[i].chan2) ||
                                (ch_mask & 1 << s->mcdparams[i].chan2))
                                return AVERROR_INVALIDDATA;

                            ch_mask |= 1 << s->mcdparams[i].chan2;
                        } else if (!(ch_mask & 1 << s->mcdparams[i].chan2)) {
                            return AVERROR_INVALIDDATA;
                        }
                    }
                    s->mcdparams[i].chan1 = nbit;

                    ch_mask |= 1 << nbit;
                }
            } else {
                chan = avctx->channels;
                for (i = 0; i < chan; i++) {
                    s->mcdparams[i].present = 0;
                    s->mcdparams[i].chan1   = i;
                }
            }

            for (i = 0; i < chan; i++) {
                if (s->mcdparams[i].present && s->mcdparams[i].index == 1)
                    if (ret = decode_channel(s, s->mcdparams[i].chan2))
                        return ret;

                if (ret = decode_channel(s, s->mcdparams[i].chan1))
                    return ret;

                if (s->mcdparams[i].present) {
                    s->dmode = mc_dmodes[s->mcdparams[i].index];
                    if (ret = decorrelate(s,
                                          s->mcdparams[i].chan2,
                                          s->mcdparams[i].chan1,
                                          s->nb_samples - 1))
                        return ret;
                }
            }
        }

        for (chan = 0; chan < avctx->channels; chan++) {
            int32_t *decoded = s->decoded[chan];

            if (s->lpc_mode[chan])
                decode_lpc(decoded, s->lpc_mode[chan], s->nb_samples);

            if (s->sample_shift[chan] > 0)
                for (i = 0; i < s->nb_samples; i++)
                    decoded[i] <<= s->sample_shift[chan];
        }
    }

    align_get_bits(gb);
    skip_bits(gb, 24);
    if (get_bits_left(gb) < 0)
        av_log(avctx, AV_LOG_DEBUG, "overread\n");
    else if (get_bits_left(gb) > 0)
        av_log(avctx, AV_LOG_DEBUG, "underread\n");

    if (avctx->err_recognition & (AV_EF_CRCCHECK | AV_EF_COMPLIANT)) {
        if (ff_tak_check_crc(pkt->data + hsize,
                             get_bits_count(gb) / 8 - hsize)) {
            av_log(avctx, AV_LOG_ERROR, "CRC error\n");
            return AVERROR_INVALIDDATA;
        }
    }

    /* convert to output buffer */
    switch (avctx->sample_fmt) {
    case AV_SAMPLE_FMT_U8P:
        for (chan = 0; chan < avctx->channels; chan++) {
            uint8_t *samples = (uint8_t *)frame->extended_data[chan];
            int32_t *decoded = s->decoded[chan];
            for (i = 0; i < s->nb_samples; i++)
                samples[i] = decoded[i] + 0x80;
        }
        break;
    case AV_SAMPLE_FMT_S16P:
        for (chan = 0; chan < avctx->channels; chan++) {
            int16_t *samples = (int16_t *)frame->extended_data[chan];
            int32_t *decoded = s->decoded[chan];
            for (i = 0; i < s->nb_samples; i++)
                samples[i] = decoded[i];
        }
        break;
    case AV_SAMPLE_FMT_S32P:
        for (chan = 0; chan < avctx->channels; chan++) {
            int32_t *samples = (int32_t *)frame->extended_data[chan];
            for (i = 0; i < s->nb_samples; i++)
                samples[i] <<= 8;
        }
        break;
    }

    *got_frame_ptr = 1;

    return pkt->size;
}
