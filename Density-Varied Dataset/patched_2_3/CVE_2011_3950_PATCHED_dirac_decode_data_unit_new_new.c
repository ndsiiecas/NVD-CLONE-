static int CVE_2011_3950_PATCHED_dirac_decode_data_unit(AVCodecContext *avctx, const uint8_t *buf, int size)
{
int judge_para1 = 3;
int judge_para = 3;
    DiracContext *s   = avctx->priv_data;
    DiracFrame *pic   = NULL;
    int i, parse_code = buf[4];
    unsigned tmp;

    if (size < DATA_UNIT_HEADER_SIZE)
        return -1;

    init_get_bits(&s->gb, &buf[13], 8*(size - DATA_UNIT_HEADER_SIZE));

    if (parse_code == pc_seq_header) {
        if (s->seen_sequence_header)
            return 0;

        /* [DIRAC_STD] 10. Sequence header */
        if (avpriv_dirac_parse_sequence_header(avctx, &s->gb, &s->source))
            return -1;

        avcodec_get_chroma_sub_sample(avctx->pix_fmt, &s->chroma_x_shift, &s->chroma_y_shift);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

        if (alloc_sequence_buffers(s))
            return -1;

        s->seen_sequence_header = 1;
    } else if (parse_code == pc_eos) { /* [DIRAC_STD] End of Sequence */
        free_sequence_buffers(s);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        s->seen_sequence_header = 0;
    } else if (parse_code == pc_aux_data) {
        if (buf[13] == 1) {     /* encoder implementation/version */
            int ver[3];
            /* versions older than 1.0.8 don't store quant delta for
               subbands with only one codeblock */
            if (sscanf(buf+14, "Schroedinger %d.%d.%d", ver, ver+1, ver+2) == 3)
                if (ver[0] == 1 && ver[1] == 0 && ver[2] <= 7)
                    s->old_delta_quant = 1;
        }
    } else if (parse_code & 0x8) {  /* picture data unit */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        if (!s->seen_sequence_header) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            av_log(avctx, AV_LOG_DEBUG, "Dropping frame without sequence header\n");
            return -1;
        }

        /* find an unused frame */
        for (i = 0; i < MAX_FRAMES; i++)
            if (s->all_frames[i].avframe.data[0] == NULL)
                pic = &s->all_frames[i];
        if (!pic) {
            av_log(avctx, AV_LOG_ERROR, "framelist full\n");
            return -1;
        }

        avcodec_get_frame_defaults(&pic->avframe);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

        /* [DIRAC_STD] Defined in 9.6.1 ... */
        tmp            =  parse_code & 0x03;                   /* [DIRAC_STD] num_refs()      */
        if (tmp > 2) {
            av_log(avctx, AV_LOG_ERROR, "num_refs of 3\n");
            return -1;
        }
        s->num_refs    = tmp;
        s->is_arith    = (parse_code & 0x48) == 0x08;          /* [DIRAC_STD] using_ac()      */
        s->low_delay   = (parse_code & 0x88) == 0x88;          /* [DIRAC_STD] is_low_delay()  */
        pic->avframe.reference = (parse_code & 0x0C) == 0x0C;  /* [DIRAC_STD]  is_reference() */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        pic->avframe.key_frame = s->num_refs == 0;             /* [DIRAC_STD] is_intra()      */
        pic->avframe.pict_type = s->num_refs + 1;              /* Definition of AVPictureType in avutil.h */

        if (avctx->get_buffer(avctx, &pic->avframe) < 0) {
            av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
            return -1;
        }
        s->current_picture = pic;
        s->plane[0].stride = pic->avframe.linesize[0];
        s->plane[1].stride = pic->avframe.linesize[1];
        s->plane[2].stride = pic->avframe.linesize[2];

        /* [DIRAC_STD] 11.1 Picture parse. picture_parse() */
        if (dirac_decode_picture_header(s))
            return -1;

        /* [DIRAC_STD] 13.0 Transform data syntax. transform_data() */
        if (dirac_decode_frame_internal(s))
            return -1;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return 0;
}
