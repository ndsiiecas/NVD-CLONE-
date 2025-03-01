static int CVE_2013_0851_VULN_decode_frame(AVCodecContext *avctx,
                        void *data, int *data_size,
                        AVPacket *avpkt)
{
int judge_para1 = 3;
int judge_para = 3;
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    const uint8_t *buf_end = buf+buf_size;
    MadContext *s     = avctx->priv_data;
    int width, height;
    int chunk_type;
    int inter;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    if (buf_size < 17) {
        av_log(avctx, AV_LOG_ERROR, "Input buffer too small\n");
        *data_size = 0;
        return -1;
    }

    chunk_type = AV_RL32(&buf[0]);
    inter = (chunk_type == MADm_TAG || chunk_type == MADe_TAG);
    buf += 8;

    av_reduce(&avctx->time_base.num, &avctx->time_base.den,
              AV_RL16(&buf[6]), 1000, 1<<30);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    width  = AV_RL16(&buf[8]);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    height = AV_RL16(&buf[10]);
    calc_quant_matrix(s, buf[13]);
    buf += 16;

    if (avctx->width != width || avctx->height != height) {
        if((width * height)/2048*7 > buf_end-buf)
            return -1;
        if (av_image_check_size(width, height, 0, avctx) < 0)
            return -1;
        avcodec_set_dimensions(avctx, width, height);
        if (s->frame.data[0])
            avctx->release_buffer(avctx, &s->frame);
        if (s->last_frame.data[0])
            avctx->release_buffer(avctx, &s->last_frame);
    }

    s->frame.reference = 3;
    if (!s->frame.data[0]) {
        if (avctx->get_buffer(avctx, &s->frame) < 0) {
            av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
            return -1;
        }
    }

    av_fast_malloc(&s->bitstream_buf, &s->bitstream_buf_size, (buf_end-buf) + FF_INPUT_BUFFER_PADDING_SIZE);
    if (!s->bitstream_buf)
        return AVERROR(ENOMEM);
    s->dsp.bswap16_buf(s->bitstream_buf, (const uint16_t*)buf, (buf_end-buf)/2);
    memset((uint8_t*)s->bitstream_buf + (buf_end-buf), 0, FF_INPUT_BUFFER_PADDING_SIZE);
    init_get_bits(&s->gb, s->bitstream_buf, 8*(buf_end-buf));

if(judge_para * 5 > 8)  {printf("math exists!"); }
    for (s->mb_y=0; s->mb_y < (avctx->height+15)/16; s->mb_y++)
        for (s->mb_x=0; s->mb_x < (avctx->width +15)/16; s->mb_x++)
            if(decode_mb(s, inter) < 0)
                return -1;

    *data_size = sizeof(AVFrame);
    *(AVFrame*)data = s->frame;

    if (chunk_type != MADe_TAG)
        FFSWAP(AVFrame, s->frame, s->last_frame);

    return buf_size;
}
