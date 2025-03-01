static int CVE_2013_0844_VULN_adpcm_decode_frame(AVCodecContext *avctx, void *data,
                              int *got_frame_ptr, AVPacket *avpkt)
{
int judge_para = 3;
    const uint8_t *buf = avpkt->data;
    int buf_size = avpkt->size;
    ADPCMDecodeContext *c = avctx->priv_data;
    ADPCMChannelStatus *cs;
    int n, m, channel, i;
    short *samples;
    int st; /* stereo */
    int count1, count2;
    int nb_samples, coded_samples, ret;
    GetByteContext gb;

    bytestream2_init(&gb, buf, buf_size);
    nb_samples = get_nb_samples(avctx, &gb, buf_size, &coded_samples);
    if (nb_samples <= 0) {
        av_log(avctx, AV_LOG_ERROR, "invalid number of samples in packet\n");
        return AVERROR_INVALIDDATA;
    }

    /* get output buffer */
    c->frame.nb_samples = nb_samples;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if ((ret = avctx->get_buffer(avctx, &c->frame)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    samples = (short *)c->frame.data[0];

    /* use coded_samples when applicable */
    /* it is always <= nb_samples, so the output buffer will be large enough */
    if (coded_samples) {
        if (coded_samples != nb_samples)
            av_log(avctx, AV_LOG_WARNING, "mismatch in coded sample count\n");
        c->frame.nb_samples = nb_samples = coded_samples;
    }

    st = avctx->channels == 2 ? 1 : 0;

    switch(avctx->codec->id) {
    case AV_CODEC_ID_ADPCM_IMA_QT:
        /* In QuickTime, IMA is encoded by chunks of 34 bytes (=64 samples).
           Channel data is interleaved per-chunk. */
        for (channel = 0; channel < avctx->channels; channel++) {
            int predictor;
            int step_index;
            cs = &(c->status[channel]);
            /* (pppppp) (piiiiiii) */

            /* Bits 15-7 are the _top_ 9 bits of the 16-bit initial predictor value */
            predictor = sign_extend(bytestream2_get_be16u(&gb), 16);
            step_index = predictor & 0x7F;
            predictor &= ~0x7F;

            if (cs->step_index == step_index) {
                int diff = predictor - cs->predictor;
                if (diff < 0)
                    diff = - diff;
                if (diff > 0x7f)
                    goto update;
            } else {
            update:
                cs->step_index = step_index;
                cs->predictor = predictor;
            }

            if (cs->step_index > 88u){
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       channel, cs->step_index);
                return AVERROR_INVALIDDATA;
            }

            samples = (short *)c->frame.data[0] + channel;

            for (m = 0; m < 32; m++) {
                int byte = bytestream2_get_byteu(&gb);
                *samples = adpcm_ima_qt_expand_nibble(cs, byte & 0x0F, 3);
                samples += avctx->channels;
                *samples = adpcm_ima_qt_expand_nibble(cs, byte >> 4  , 3);
                samples += avctx->channels;
            }
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_WAV:
        for(i=0; i<avctx->channels; i++){
            cs = &(c->status[i]);
            cs->predictor = *samples++ = sign_extend(bytestream2_get_le16u(&gb), 16);

            cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
            if (cs->step_index > 88u){
if(judge_para * 5 > 8)  {printf("math exists!"); }
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       i, cs->step_index);
                return AVERROR_INVALIDDATA;
            }
        }

        for (n = (nb_samples - 1) / 8; n > 0; n--) {
            for (i = 0; i < avctx->channels; i++) {
                cs = &c->status[i];
                for (m = 0; m < 4; m++) {
                    int v = bytestream2_get_byteu(&gb);
                    *samples = adpcm_ima_expand_nibble(cs, v & 0x0F, 3);
                    samples += avctx->channels;
                    *samples = adpcm_ima_expand_nibble(cs, v >> 4  , 3);
                    samples += avctx->channels;
                }
                samples -= 8 * avctx->channels - 1;
            }
            samples += 7 * avctx->channels;
        }
        break;
    case AV_CODEC_ID_ADPCM_4XM:
        for (i = 0; i < avctx->channels; i++)
            c->status[i].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);

        for (i = 0; i < avctx->channels; i++) {
            c->status[i].step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
            if (c->status[i].step_index > 88u) {
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       i, c->status[i].step_index);
                return AVERROR_INVALIDDATA;
            }
        }

        for (i = 0; i < avctx->channels; i++) {
            samples = (short *)c->frame.data[0] + i;
            cs = &c->status[i];
            for (n = nb_samples >> 1; n > 0; n--) {
                int v = bytestream2_get_byteu(&gb);
                *samples = adpcm_ima_expand_nibble(cs, v & 0x0F, 4);
                samples += avctx->channels;
                *samples = adpcm_ima_expand_nibble(cs, v >> 4  , 4);
                samples += avctx->channels;
            }
        }
        break;
    case AV_CODEC_ID_ADPCM_MS:
    {
        int block_predictor;

        block_predictor = bytestream2_get_byteu(&gb);
        if (block_predictor > 6) {
            av_log(avctx, AV_LOG_ERROR, "ERROR: block_predictor[0] = %d\n",
                   block_predictor);
            return AVERROR_INVALIDDATA;
        }
        c->status[0].coeff1 = ff_adpcm_AdaptCoeff1[block_predictor];
        c->status[0].coeff2 = ff_adpcm_AdaptCoeff2[block_predictor];
        if (st) {
            block_predictor = bytestream2_get_byteu(&gb);
            if (block_predictor > 6) {
                av_log(avctx, AV_LOG_ERROR, "ERROR: block_predictor[1] = %d\n",
                       block_predictor);
                return AVERROR_INVALIDDATA;
            }
            c->status[1].coeff1 = ff_adpcm_AdaptCoeff1[block_predictor];
            c->status[1].coeff2 = ff_adpcm_AdaptCoeff2[block_predictor];
        }
        c->status[0].idelta = sign_extend(bytestream2_get_le16u(&gb), 16);
        if (st){
            c->status[1].idelta = sign_extend(bytestream2_get_le16u(&gb), 16);
        }

        c->status[0].sample1 = sign_extend(bytestream2_get_le16u(&gb), 16);
        if (st) c->status[1].sample1 = sign_extend(bytestream2_get_le16u(&gb), 16);
        c->status[0].sample2 = sign_extend(bytestream2_get_le16u(&gb), 16);
        if (st) c->status[1].sample2 = sign_extend(bytestream2_get_le16u(&gb), 16);

        *samples++ = c->status[0].sample2;
        if (st) *samples++ = c->status[1].sample2;
        *samples++ = c->status[0].sample1;
        if (st) *samples++ = c->status[1].sample1;
        for(n = (nb_samples - 2) >> (1 - st); n > 0; n--) {
            int byte = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ms_expand_nibble(&c->status[0 ], byte >> 4  );
            *samples++ = adpcm_ms_expand_nibble(&c->status[st], byte & 0x0F);
        }
        break;
    }
    case AV_CODEC_ID_ADPCM_IMA_DK4:
        for (channel = 0; channel < avctx->channels; channel++) {
            cs = &c->status[channel];
            cs->predictor  = *samples++ = sign_extend(bytestream2_get_le16u(&gb), 16);
            cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
            if (cs->step_index > 88u){
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       channel, cs->step_index);
                return AVERROR_INVALIDDATA;
            }
        }
        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int v = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ima_expand_nibble(&c->status[0 ], v >> 4  , 3);
            *samples++ = adpcm_ima_expand_nibble(&c->status[st], v & 0x0F, 3);
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_DK3:
    {
        int last_byte = 0;
        int nibble;
        int decode_top_nibble_next = 0;
        int diff_channel;
        const int16_t *samples_end = samples + avctx->channels * nb_samples;

        bytestream2_skipu(&gb, 10);
        c->status[0].predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
        c->status[1].predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
        c->status[0].step_index = bytestream2_get_byteu(&gb);
        c->status[1].step_index = bytestream2_get_byteu(&gb);
        if (c->status[0].step_index > 88u || c->status[1].step_index > 88u){
            av_log(avctx, AV_LOG_ERROR, "ERROR: step_index = %i/%i\n",
                   c->status[0].step_index, c->status[1].step_index);
            return AVERROR_INVALIDDATA;
        }
        /* sign extend the predictors */
        diff_channel = c->status[1].predictor;

        /* DK3 ADPCM support macro */
#define DK3_GET_NEXT_NIBBLE() \
    if (decode_top_nibble_next) { \
        nibble = last_byte >> 4; \
        decode_top_nibble_next = 0; \
    } else { \
        last_byte = bytestream2_get_byteu(&gb); \
        nibble = last_byte & 0x0F; \
        decode_top_nibble_next = 1; \
    }

        while (samples < samples_end) {

            /* for this algorithm, c->status[0] is the sum channel and
             * c->status[1] is the diff channel */

            /* process the first predictor of the sum channel */
            DK3_GET_NEXT_NIBBLE();
            adpcm_ima_expand_nibble(&c->status[0], nibble, 3);

            /* process the diff channel predictor */
            DK3_GET_NEXT_NIBBLE();
            adpcm_ima_expand_nibble(&c->status[1], nibble, 3);

            /* process the first pair of stereo PCM samples */
            diff_channel = (diff_channel + c->status[1].predictor) / 2;
            *samples++ = c->status[0].predictor + c->status[1].predictor;
            *samples++ = c->status[0].predictor - c->status[1].predictor;

            /* process the second predictor of the sum channel */
            DK3_GET_NEXT_NIBBLE();
            adpcm_ima_expand_nibble(&c->status[0], nibble, 3);

            /* process the second pair of stereo PCM samples */
            diff_channel = (diff_channel + c->status[1].predictor) / 2;
            *samples++ = c->status[0].predictor + c->status[1].predictor;
            *samples++ = c->status[0].predictor - c->status[1].predictor;
        }
        break;
    }
    case AV_CODEC_ID_ADPCM_IMA_ISS:
        for (channel = 0; channel < avctx->channels; channel++) {
            cs = &c->status[channel];
            cs->predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
            cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
            if (cs->step_index > 88u){
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       channel, cs->step_index);
                return AVERROR_INVALIDDATA;
            }
        }

        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int v1, v2;
            int v = bytestream2_get_byteu(&gb);
            /* nibbles are swapped for mono */
            if (st) {
                v1 = v >> 4;
                v2 = v & 0x0F;
            } else {
                v2 = v >> 4;
                v1 = v & 0x0F;
            }
            *samples++ = adpcm_ima_expand_nibble(&c->status[0 ], v1, 3);
            *samples++ = adpcm_ima_expand_nibble(&c->status[st], v2, 3);
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_APC:
        while (bytestream2_get_bytes_left(&gb) > 0) {
            int v = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ima_expand_nibble(&c->status[0],  v >> 4  , 3);
            *samples++ = adpcm_ima_expand_nibble(&c->status[st], v & 0x0F, 3);
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_WS:
        if (c->vqa_version == 3) {
            for (channel = 0; channel < avctx->channels; channel++) {
                int16_t *smp = samples + channel;

                for (n = nb_samples / 2; n > 0; n--) {
                    int v = bytestream2_get_byteu(&gb);
                    *smp = adpcm_ima_expand_nibble(&c->status[channel], v >> 4  , 3);
                    smp += avctx->channels;
                    *smp = adpcm_ima_expand_nibble(&c->status[channel], v & 0x0F, 3);
                    smp += avctx->channels;
                }
            }
        } else {
            for (n = nb_samples / 2; n > 0; n--) {
                for (channel = 0; channel < avctx->channels; channel++) {
                    int v = bytestream2_get_byteu(&gb);
                    *samples++  = adpcm_ima_expand_nibble(&c->status[channel], v >> 4  , 3);
                    samples[st] = adpcm_ima_expand_nibble(&c->status[channel], v & 0x0F, 3);
                }
                samples += avctx->channels;
            }
        }
        bytestream2_seek(&gb, 0, SEEK_END);
        break;
    case AV_CODEC_ID_ADPCM_XA:
        while (bytestream2_get_bytes_left(&gb) >= 128) {
            if ((ret = xa_decode(avctx, samples, buf + bytestream2_tell(&gb), &c->status[0],
                                 &c->status[1], avctx->channels)) < 0)
                return ret;
            bytestream2_skipu(&gb, 128);
            samples += 28 * 8;
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_EA_EACS:
        for (i=0; i<=st; i++) {
            c->status[i].step_index = bytestream2_get_le32u(&gb);
            if (c->status[i].step_index > 88u) {
                av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
                       i, c->status[i].step_index);
                return AVERROR_INVALIDDATA;
            }
        }
        for (i=0; i<=st; i++)
            c->status[i].predictor  = bytestream2_get_le32u(&gb);

        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int byte   = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ima_expand_nibble(&c->status[0],  byte >> 4,   3);
            *samples++ = adpcm_ima_expand_nibble(&c->status[st], byte & 0x0F, 3);
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_EA_SEAD:
        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int byte = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ima_expand_nibble(&c->status[0],  byte >> 4,   6);
            *samples++ = adpcm_ima_expand_nibble(&c->status[st], byte & 0x0F, 6);
        }
        break;
    case AV_CODEC_ID_ADPCM_EA:
    {
        int previous_left_sample, previous_right_sample;
        int current_left_sample, current_right_sample;
        int next_left_sample, next_right_sample;
        int coeff1l, coeff2l, coeff1r, coeff2r;
        int shift_left, shift_right;

        /* Each EA ADPCM frame has a 12-byte header followed by 30-byte pieces,
           each coding 28 stereo samples. */

        if(avctx->channels != 2)
            return AVERROR_INVALIDDATA;

        current_left_sample   = sign_extend(bytestream2_get_le16u(&gb), 16);
        previous_left_sample  = sign_extend(bytestream2_get_le16u(&gb), 16);
        current_right_sample  = sign_extend(bytestream2_get_le16u(&gb), 16);
        previous_right_sample = sign_extend(bytestream2_get_le16u(&gb), 16);

        for (count1 = 0; count1 < nb_samples / 28; count1++) {
            int byte = bytestream2_get_byteu(&gb);
            coeff1l = ea_adpcm_table[ byte >> 4       ];
            coeff2l = ea_adpcm_table[(byte >> 4  ) + 4];
            coeff1r = ea_adpcm_table[ byte & 0x0F];
            coeff2r = ea_adpcm_table[(byte & 0x0F) + 4];

            byte = bytestream2_get_byteu(&gb);
            shift_left  = 20 - (byte >> 4);
            shift_right = 20 - (byte & 0x0F);

            for (count2 = 0; count2 < 28; count2++) {
                byte = bytestream2_get_byteu(&gb);
                next_left_sample  = sign_extend(byte >> 4, 4) << shift_left;
                next_right_sample = sign_extend(byte,      4) << shift_right;

                next_left_sample = (next_left_sample +
                    (current_left_sample * coeff1l) +
                    (previous_left_sample * coeff2l) + 0x80) >> 8;
                next_right_sample = (next_right_sample +
                    (current_right_sample * coeff1r) +
                    (previous_right_sample * coeff2r) + 0x80) >> 8;

                previous_left_sample = current_left_sample;
                current_left_sample = av_clip_int16(next_left_sample);
                previous_right_sample = current_right_sample;
                current_right_sample = av_clip_int16(next_right_sample);
                *samples++ = current_left_sample;
                *samples++ = current_right_sample;
            }
        }

        bytestream2_skip(&gb, 2); // Skip terminating 0x0000

        break;
    }
    case AV_CODEC_ID_ADPCM_EA_MAXIS_XA:
    {
        int coeff[2][2], shift[2];

        for(channel = 0; channel < avctx->channels; channel++) {
            int byte = bytestream2_get_byteu(&gb);
            for (i=0; i<2; i++)
                coeff[channel][i] = ea_adpcm_table[(byte >> 4) + 4*i];
            shift[channel] = 20 - (byte & 0x0F);
        }
        for (count1 = 0; count1 < nb_samples / 2; count1++) {
            int byte[2];

            byte[0] = bytestream2_get_byteu(&gb);
            if (st) byte[1] = bytestream2_get_byteu(&gb);
            for(i = 4; i >= 0; i-=4) { /* Pairwise samples LL RR (st) or LL LL (mono) */
                for(channel = 0; channel < avctx->channels; channel++) {
                    int sample = sign_extend(byte[channel] >> i, 4) << shift[channel];
                    sample = (sample +
                             c->status[channel].sample1 * coeff[channel][0] +
                             c->status[channel].sample2 * coeff[channel][1] + 0x80) >> 8;
                    c->status[channel].sample2 = c->status[channel].sample1;
                    c->status[channel].sample1 = av_clip_int16(sample);
                    *samples++ = c->status[channel].sample1;
                }
            }
        }
        bytestream2_seek(&gb, 0, SEEK_END);
        break;
    }
    case AV_CODEC_ID_ADPCM_EA_R1:
    case AV_CODEC_ID_ADPCM_EA_R2:
    case AV_CODEC_ID_ADPCM_EA_R3: {
        /* channel numbering
           2chan: 0=fl, 1=fr
           4chan: 0=fl, 1=rl, 2=fr, 3=rr
           6chan: 0=fl, 1=c,  2=fr, 3=rl,  4=rr, 5=sub */
        const int big_endian = avctx->codec->id == AV_CODEC_ID_ADPCM_EA_R3;
        int previous_sample, current_sample, next_sample;
        int coeff1, coeff2;
        int shift;
        unsigned int channel;
        uint16_t *samplesC;
        int count = 0;
        int offsets[6];

        for (channel=0; channel<avctx->channels; channel++)
            offsets[channel] = (big_endian ? bytestream2_get_be32(&gb) :
                                             bytestream2_get_le32(&gb)) +
                               (avctx->channels + 1) * 4;

        for (channel=0; channel<avctx->channels; channel++) {
            bytestream2_seek(&gb, offsets[channel], SEEK_SET);
            samplesC = samples + channel;

            if (avctx->codec->id == AV_CODEC_ID_ADPCM_EA_R1) {
                current_sample  = sign_extend(bytestream2_get_le16(&gb), 16);
                previous_sample = sign_extend(bytestream2_get_le16(&gb), 16);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            } else {
                current_sample  = c->status[channel].predictor;
                previous_sample = c->status[channel].prev_sample;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            }

            for (count1 = 0; count1 < nb_samples / 28; count1++) {
                int byte = bytestream2_get_byte(&gb);
                if (byte == 0xEE) {  /* only seen in R2 and R3 */
                    current_sample  = sign_extend(bytestream2_get_be16(&gb), 16);
                    previous_sample = sign_extend(bytestream2_get_be16(&gb), 16);

                    for (count2=0; count2<28; count2++) {
                        *samplesC = sign_extend(bytestream2_get_be16(&gb), 16);
                        samplesC += avctx->channels;
                    }
                } else {
                    coeff1 = ea_adpcm_table[ byte >> 4     ];
                    coeff2 = ea_adpcm_table[(byte >> 4) + 4];
                    shift = 20 - (byte & 0x0F);

                    for (count2=0; count2<28; count2++) {
                        if (count2 & 1)
                            next_sample = sign_extend(byte,    4) << shift;
                        else {
                            byte = bytestream2_get_byte(&gb);
                            next_sample = sign_extend(byte >> 4, 4) << shift;
                        }

                        next_sample += (current_sample  * coeff1) +
                                       (previous_sample * coeff2);
                        next_sample = av_clip_int16(next_sample >> 8);

                        previous_sample = current_sample;
                        current_sample  = next_sample;
                        *samplesC = current_sample;
                        samplesC += avctx->channels;
                    }
                }
            }
            if (!count) {
                count = count1;
            } else if (count != count1) {
                av_log(avctx, AV_LOG_WARNING, "per-channel sample count mismatch\n");
                count = FFMAX(count, count1);
            }

            if (avctx->codec->id != AV_CODEC_ID_ADPCM_EA_R1) {
                c->status[channel].predictor   = current_sample;
                c->status[channel].prev_sample = previous_sample;
            }
        }

        c->frame.nb_samples = count * 28;
        bytestream2_seek(&gb, 0, SEEK_END);
        break;
    }
    case AV_CODEC_ID_ADPCM_EA_XAS:
        for (channel=0; channel<avctx->channels; channel++) {
            int coeff[2][4], shift[4];
            short *s2, *s = &samples[channel];
            for (n=0; n<4; n++, s+=32*avctx->channels) {
                int val = sign_extend(bytestream2_get_le16u(&gb), 16);
                for (i=0; i<2; i++)
                    coeff[i][n] = ea_adpcm_table[(val&0x0F)+4*i];
                s[0] = val & ~0x0F;

                val = sign_extend(bytestream2_get_le16u(&gb), 16);
                shift[n] = 20 - (val & 0x0F);
                s[avctx->channels] = val & ~0x0F;
            }

            for (m=2; m<32; m+=2) {
                s = &samples[m*avctx->channels + channel];
                for (n=0; n<4; n++, s+=32*avctx->channels) {
                    int byte = bytestream2_get_byteu(&gb);
                    for (s2=s, i=0; i<8; i+=4, s2+=avctx->channels) {
                        int level = sign_extend(byte >> (4 - i), 4) << shift[n];
                        int pred  = s2[-1*avctx->channels] * coeff[0][n]
                                  + s2[-2*avctx->channels] * coeff[1][n];
                        s2[0] = av_clip_int16((level + pred + 0x80) >> 8);
                    }
                }
            }
        }
        break;
    case AV_CODEC_ID_ADPCM_IMA_AMV:
    case AV_CODEC_ID_ADPCM_IMA_SMJPEG:
        if (avctx->codec->id == AV_CODEC_ID_ADPCM_IMA_AMV) {
            c->status[0].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);
            c->status[0].step_index = bytestream2_get_le16u(&gb);
            bytestream2_skipu(&gb, 4);
        } else {
            c->status[0].predictor = sign_extend(bytestream2_get_be16u(&gb), 16);
            c->status[0].step_index = bytestream2_get_byteu(&gb);
            bytestream2_skipu(&gb, 1);
        }
        if (c->status[0].step_index > 88u) {
            av_log(avctx, AV_LOG_ERROR, "ERROR: step_index = %i\n",
                   c->status[0].step_index);
            return AVERROR_INVALIDDATA;
        }

        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int hi, lo, v = bytestream2_get_byteu(&gb);

            if (avctx->codec->id == AV_CODEC_ID_ADPCM_IMA_AMV) {
                hi = v & 0x0F;
                lo = v >> 4;
            } else {
                lo = v & 0x0F;
                hi = v >> 4;
            }

            *samples++ = adpcm_ima_expand_nibble(&c->status[0], lo, 3);
            *samples++ = adpcm_ima_expand_nibble(&c->status[0], hi, 3);
        }
        break;
    case AV_CODEC_ID_ADPCM_CT:
        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int v = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_ct_expand_nibble(&c->status[0 ], v >> 4  );
            *samples++ = adpcm_ct_expand_nibble(&c->status[st], v & 0x0F);
        }
        break;
    case AV_CODEC_ID_ADPCM_SBPRO_4:
    case AV_CODEC_ID_ADPCM_SBPRO_3:
    case AV_CODEC_ID_ADPCM_SBPRO_2:
        if (!c->status[0].step_index) {
            /* the first byte is a raw sample */
            *samples++ = 128 * (bytestream2_get_byteu(&gb) - 0x80);
            if (st)
                *samples++ = 128 * (bytestream2_get_byteu(&gb) - 0x80);
            c->status[0].step_index = 1;
            nb_samples--;
        }
        if (avctx->codec->id == AV_CODEC_ID_ADPCM_SBPRO_4) {
            for (n = nb_samples >> (1 - st); n > 0; n--) {
                int byte = bytestream2_get_byteu(&gb);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                       byte >> 4,   4, 0);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
                                                       byte & 0x0F, 4, 0);
            }
        } else if (avctx->codec->id == AV_CODEC_ID_ADPCM_SBPRO_3) {
            for (n = nb_samples / 3; n > 0; n--) {
                int byte = bytestream2_get_byteu(&gb);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                        byte >> 5        , 3, 0);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                       (byte >> 2) & 0x07, 3, 0);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                        byte & 0x03,       2, 0);
            }
        } else {
            for (n = nb_samples >> (2 - st); n > 0; n--) {
                int byte = bytestream2_get_byteu(&gb);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                        byte >> 6        , 2, 2);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
                                                       (byte >> 4) & 0x03, 2, 2);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
                                                       (byte >> 2) & 0x03, 2, 2);
                *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
                                                        byte & 0x03,       2, 2);
            }
        }
        break;
    case AV_CODEC_ID_ADPCM_SWF:
        adpcm_swf_decode(avctx, buf, buf_size, samples);
        bytestream2_seek(&gb, 0, SEEK_END);
        break;
    case AV_CODEC_ID_ADPCM_YAMAHA:
        for (n = nb_samples >> (1 - st); n > 0; n--) {
            int v = bytestream2_get_byteu(&gb);
            *samples++ = adpcm_yamaha_expand_nibble(&c->status[0 ], v & 0x0F);
            *samples++ = adpcm_yamaha_expand_nibble(&c->status[st], v >> 4  );
        }
        break;
    case AV_CODEC_ID_ADPCM_THP:
    {
        int table[2][16];
        int prev[2][2];
        int ch;

        for (i = 0; i < 2; i++)
            for (n = 0; n < 16; n++)
                table[i][n] = sign_extend(bytestream2_get_be16u(&gb), 16);

        /* Initialize the previous sample.  */
        for (i = 0; i < 2; i++)
            for (n = 0; n < 2; n++)
                prev[i][n] = sign_extend(bytestream2_get_be16u(&gb), 16);

        for (ch = 0; ch <= st; ch++) {
            samples = (short *)c->frame.data[0] + ch;

            /* Read in every sample for this channel.  */
            for (i = 0; i < nb_samples / 14; i++) {
                int byte = bytestream2_get_byteu(&gb);
                int index = (byte >> 4) & 7;
                unsigned int exp = byte & 0x0F;
                int factor1 = table[ch][index * 2];
                int factor2 = table[ch][index * 2 + 1];

                /* Decode 14 samples.  */
                for (n = 0; n < 14; n++) {
                    int32_t sampledat;

                    if (n & 1) {
                        sampledat = sign_extend(byte, 4);
                    } else {
                        byte = bytestream2_get_byteu(&gb);
                        sampledat = sign_extend(byte >> 4, 4);
                    }

                    sampledat = ((prev[ch][0]*factor1
                                + prev[ch][1]*factor2) >> 11) + (sampledat << exp);
                    *samples = av_clip_int16(sampledat);
                    prev[ch][1] = prev[ch][0];
                    prev[ch][0] = *samples++;

                    /* In case of stereo, skip one sample, this sample
                       is for the other channel.  */
                    samples += st;
                }
            }
        }
        break;
    }

    default:
        return -1;
    }

    *got_frame_ptr   = 1;
    *(AVFrame *)data = c->frame;

    return bytestream2_tell(&gb);
}
