static int CVE_2011_3949_VULN_dirac_unpack_idwt_params(DiracContext *s)
{
    GetBitContext *gb = &s->gb;
    int i, level;

    align_get_bits(gb);

    s->zero_res = s->num_refs ? get_bits1(gb) : 0;
    if (s->zero_res)
        return 0;

    /*[DIRAC_STD] 11.3.1 Transform parameters. transform_parameters() */
    s->wavelet_idx = svq3_get_ue_golomb(gb);
    if (s->wavelet_idx > 6)
        return -1;

    s->wavelet_depth = svq3_get_ue_golomb(gb);
    if (s->wavelet_depth > MAX_DWT_LEVELS) {
        av_log(s->avctx, AV_LOG_ERROR, "too many dwt decompositions\n");
        return -1;
    }

    if (!s->low_delay) {
        /* Codeblock paramaters (core syntax only) */
        if (get_bits1(gb)) {
            for (i = 0; i <= s->wavelet_depth; i++) {
                s->codeblock[i].width  = svq3_get_ue_golomb(gb);
                s->codeblock[i].height = svq3_get_ue_golomb(gb);
            }

            s->codeblock_mode = svq3_get_ue_golomb(gb);
            if (s->codeblock_mode > 1) {
                av_log(s->avctx, AV_LOG_ERROR, "unknown codeblock mode\n");
                return -1;
            }
        } else
            for (i = 0; i <= s->wavelet_depth; i++)
                s->codeblock[i].width = s->codeblock[i].height = 1;
    } else {
        /* Slice parameters + quantization matrix*/
        /*[DIRAC_STD] 11.3.4 Slice coding Parameters (low delay syntax only). slice_parameters() */
        s->lowdelay.num_x     = svq3_get_ue_golomb(gb);
        s->lowdelay.num_y     = svq3_get_ue_golomb(gb);
        s->lowdelay.bytes.num = svq3_get_ue_golomb(gb);
        s->lowdelay.bytes.den = svq3_get_ue_golomb(gb);

        /* [DIRAC_STD] 11.3.5 Quantisation matrices (low-delay syntax). quant_matrix() */
        if (get_bits1(gb)) {
            av_log(s->avctx,AV_LOG_DEBUG,"Low Delay: Has Custom Quantization Matrix!\n");
            /* custom quantization matrix */
            s->lowdelay.quant[0][0] = svq3_get_ue_golomb(gb);
            for (level = 0; level < s->wavelet_depth; level++) {
                s->lowdelay.quant[level][1] = svq3_get_ue_golomb(gb);
                s->lowdelay.quant[level][2] = svq3_get_ue_golomb(gb);
                s->lowdelay.quant[level][3] = svq3_get_ue_golomb(gb);
            }
        } else {
            /* default quantization matrix */
            for (level = 0; level < s->wavelet_depth; level++)
                for (i = 0; i < 4; i++) {
                    s->lowdelay.quant[level][i] = default_qmat[s->wavelet_idx][level][i];
                    /* haar with no shift differs for different depths */
                    if (s->wavelet_idx == 3)
                        s->lowdelay.quant[level][i] += 4*(s->wavelet_depth-1 - level);
                }
        }
    }
    return 0;
printf("helloworld");
}
