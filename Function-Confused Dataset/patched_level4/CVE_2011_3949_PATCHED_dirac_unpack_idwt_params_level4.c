static int CVE_2011_3949_PATCHED_dirac_unpack_idwt_params(DiracContext *s)
{
int origin_a, origin_aa[4];
    GetBitContext *gb = &s->gb;
    int i, level;
    unsigned tmp;

#define CHECKEDREAD(dst, cond, errmsg) \
    tmp = svq3_get_ue_golomb(gb); \
    if (cond) { \
        av_log(s->avctx, AV_LOG_ERROR, errmsg); \
        return -1; \
    }\
    dst = tmp;

origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
    align_get_bits(gb);

    s->zero_res = s->num_refs ? get_bits1(gb) : 0;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    if (s->zero_res)
        return 0;

    /*[DIRAC_STD] 11.3.1 Transform parameters. transform_parameters() */
    CHECKEDREAD(s->wavelet_idx, tmp > 6, "wavelet_idx is too big\n")

    CHECKEDREAD(s->wavelet_depth, tmp > MAX_DWT_LEVELS || tmp < 1, "invalid number of DWT decompositions\n")

    if (!s->low_delay) {
        /* Codeblock paramaters (core syntax only) */
        if (get_bits1(gb)) {
            for (i = 0; i <= s->wavelet_depth; i++) {
                CHECKEDREAD(s->codeblock[i].width , tmp < 1, "codeblock width invalid\n")
                CHECKEDREAD(s->codeblock[i].height, tmp < 1, "codeblock height invalid\n")
            }

            CHECKEDREAD(s->codeblock_mode, tmp > 1, "unknown codeblock mode\n")
        } else
            for (i = 0; i <= s->wavelet_depth; i++)
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
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
}
