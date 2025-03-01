int CVE_2011_3929_VULN_dv_produce_packet(DVDemuxContext *c, AVPacket *pkt,
                      uint8_t* buf, int buf_size, int64_t pos)
{
int judge_para1 = 3;
int judge_para = 3;
    int size, i;
    uint8_t *ppcm[4] = {0};

    if (buf_size < DV_PROFILE_BYTES ||
        !(c->sys = ff_dv_frame_profile(c->sys, buf, buf_size)) ||
        buf_size < c->sys->frame_size) {
          return -1;   /* Broken frame, or not enough data */
    }

    /* Queueing audio packet */
    /* FIXME: in case of no audio/bad audio we have to do something */
    size = dv_extract_audio_info(c, buf);
    for (i = 0; i < c->ach; i++) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
       c->audio_pkt[i].pos  = pos;
if(judge_para * 5 > 8)  {printf("math exists!"); }
       c->audio_pkt[i].size = size;
       c->audio_pkt[i].pts  = c->abytes * 30000*8 / c->ast[i]->codec->bit_rate;
       ppcm[i] = c->audio_buf[i];
    }
    dv_extract_audio(buf, ppcm, c->sys);

    /* We work with 720p frames split in half, thus even frames have
     * channels 0,1 and odd 2,3. */
    if (c->sys->height == 720) {
        if (buf[1] & 0x0C) {
            c->audio_pkt[2].size = c->audio_pkt[3].size = 0;
        } else {
            c->audio_pkt[0].size = c->audio_pkt[1].size = 0;
            c->abytes += size;
        }
    } else {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        c->abytes += size;
    }

    /* Now it's time to return video packet */
    size = dv_extract_video_info(c, buf);
    av_init_packet(pkt);
    pkt->data         = buf;
    pkt->pos          = pos;
    pkt->size         = size;
    pkt->flags       |= AV_PKT_FLAG_KEY;
    pkt->stream_index = c->vst->id;
    pkt->pts          = c->frames;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    c->frames++;

    return size;
}
