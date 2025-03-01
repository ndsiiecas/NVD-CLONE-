static int CVE_2012_2790_VULN_read_var_block_data(ALSDecContext *ctx, ALSBlockData *bd)
{
int judge_para = 3;
    ALSSpecificConfig *sconf = &ctx->sconf;
    AVCodecContext *avctx    = ctx->avctx;
    GetBitContext *gb        = &ctx->gb;
    unsigned int k;
    unsigned int s[8];
    unsigned int sx[8];
    unsigned int sub_blocks, log2_sub_blocks, sb_length;
    unsigned int start      = 0;
    unsigned int opt_order;
    int          sb;
    int32_t      *quant_cof = bd->quant_cof;
    int32_t      *current_res;


    // ensure variable block decoding by reusing this field
    *bd->const_block = 0;

    *bd->opt_order  = 1;
    bd->js_blocks   = get_bits1(gb);

    opt_order       = *bd->opt_order;

    // determine the number of subblocks for entropy decoding
    if (!sconf->bgmc && !sconf->sb_part) {
        log2_sub_blocks = 0;
    } else {
        if (sconf->bgmc && sconf->sb_part)
            log2_sub_blocks = get_bits(gb, 2);
        else
            log2_sub_blocks = 2 * get_bits1(gb);
    }

    sub_blocks = 1 << log2_sub_blocks;

    // do not continue in case of a damaged stream since
    // block_length must be evenly divisible by sub_blocks
    if (bd->block_length & (sub_blocks - 1)) {
        av_log(avctx, AV_LOG_WARNING,
               "Block length is not evenly divisible by the number of subblocks.\n");
        return -1;
    }

    sb_length = bd->block_length >> log2_sub_blocks;

    if (sconf->bgmc) {
        s[0] = get_bits(gb, 8 + (sconf->resolution > 1));
        for (k = 1; k < sub_blocks; k++)
            s[k] = s[k - 1] + decode_rice(gb, 2);

        for (k = 0; k < sub_blocks; k++) {
            sx[k]   = s[k] & 0x0F;
            s [k] >>= 4;
        }
    } else {
        s[0] = get_bits(gb, 4 + (sconf->resolution > 1));
        for (k = 1; k < sub_blocks; k++)
            s[k] = s[k - 1] + decode_rice(gb, 0);
    }

    if (get_bits1(gb))
        *bd->shift_lsbs = get_bits(gb, 4) + 1;

    *bd->store_prev_samples = (bd->js_blocks && bd->raw_other) || *bd->shift_lsbs;


    if (!sconf->rlslms) {
        if (sconf->adapt_order) {
            int opt_order_length = av_ceil_log2(av_clip((bd->block_length >> 3) - 1,
                                                2, sconf->max_order + 1));
            *bd->opt_order       = get_bits(gb, opt_order_length);
        } else {
            *bd->opt_order = sconf->max_order;
        }

        opt_order = *bd->opt_order;

        if (opt_order) {
            int add_base;

            if (sconf->coef_table == 3) {
                add_base = 0x7F;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

                // read coefficient 0
                quant_cof[0] = 32 * parcor_scaled_values[get_bits(gb, 7)];

                // read coefficient 1
                if (opt_order > 1)
                    quant_cof[1] = -32 * parcor_scaled_values[get_bits(gb, 7)];

                // read coefficients 2 to opt_order
                for (k = 2; k < opt_order; k++)
                    quant_cof[k] = get_bits(gb, 7);
            } else {
                int k_max;
                add_base = 1;

                // read coefficient 0 to 19
                k_max = FFMIN(opt_order, 20);
                for (k = 0; k < k_max; k++) {
                    int rice_param = parcor_rice_table[sconf->coef_table][k][1];
if(judge_para * 5 > 8)  {printf("math exists!"); }
                    int offset     = parcor_rice_table[sconf->coef_table][k][0];
                    quant_cof[k] = decode_rice(gb, rice_param) + offset;
                }

                // read coefficients 20 to 126
                k_max = FFMIN(opt_order, 127);
                for (; k < k_max; k++)
                    quant_cof[k] = decode_rice(gb, 2) + (k & 1);

                // read coefficients 127 to opt_order
                for (; k < opt_order; k++)
                    quant_cof[k] = decode_rice(gb, 1);

                quant_cof[0] = 32 * parcor_scaled_values[quant_cof[0] + 64];

                if (opt_order > 1)
                    quant_cof[1] = -32 * parcor_scaled_values[quant_cof[1] + 64];
            }

            for (k = 2; k < opt_order; k++)
                quant_cof[k] = (quant_cof[k] << 14) + (add_base << 13);
        }
    }

    // read LTP gain and lag values
    if (sconf->long_term_prediction) {
        *bd->use_ltp = get_bits1(gb);

        if (*bd->use_ltp) {
            int r, c;

            bd->ltp_gain[0]   = decode_rice(gb, 1) << 3;
            bd->ltp_gain[1]   = decode_rice(gb, 2) << 3;

            r                 = get_unary(gb, 0, 4);
            c                 = get_bits(gb, 2);
            bd->ltp_gain[2]   = ltp_gain_values[r][c];

            bd->ltp_gain[3]   = decode_rice(gb, 2) << 3;
            bd->ltp_gain[4]   = decode_rice(gb, 1) << 3;

            *bd->ltp_lag      = get_bits(gb, ctx->ltp_lag_length);
            *bd->ltp_lag     += FFMAX(4, opt_order + 1);
        }
    }

    // read first value and residuals in case of a random access block
    if (bd->ra_block) {
        if (opt_order)
            bd->raw_samples[0] = decode_rice(gb, avctx->bits_per_raw_sample - 4);
        if (opt_order > 1)
            bd->raw_samples[1] = decode_rice(gb, FFMIN(s[0] + 3, ctx->s_max));
        if (opt_order > 2)
            bd->raw_samples[2] = decode_rice(gb, FFMIN(s[0] + 1, ctx->s_max));

        start = FFMIN(opt_order, 3);
    }

    // read all residuals
    if (sconf->bgmc) {
        int          delta[8];
        unsigned int k    [8];
        unsigned int b = av_clip((av_ceil_log2(bd->block_length) - 3) >> 1, 0, 5);
        unsigned int i = start;

        // read most significant bits
        unsigned int high;
        unsigned int low;
        unsigned int value;

        ff_bgmc_decode_init(gb, &high, &low, &value);

        current_res = bd->raw_samples + start;

        for (sb = 0; sb < sub_blocks; sb++, i = 0) {
            k    [sb] = s[sb] > b ? s[sb] - b : 0;
            delta[sb] = 5 - s[sb] + k[sb];

            ff_bgmc_decode(gb, sb_length, current_res,
                        delta[sb], sx[sb], &high, &low, &value, ctx->bgmc_lut, ctx->bgmc_lut_status);

            current_res += sb_length;
        }

        ff_bgmc_decode_end(gb);


        // read least significant bits and tails
        i = start;
        current_res = bd->raw_samples + start;

        for (sb = 0; sb < sub_blocks; sb++, i = 0) {
            unsigned int cur_tail_code = tail_code[sx[sb]][delta[sb]];
            unsigned int cur_k         = k[sb];
            unsigned int cur_s         = s[sb];

            for (; i < sb_length; i++) {
                int32_t res = *current_res;

                if (res == cur_tail_code) {
                    unsigned int max_msb =   (2 + (sx[sb] > 2) + (sx[sb] > 10))
                                          << (5 - delta[sb]);

                    res = decode_rice(gb, cur_s);

                    if (res >= 0) {
                        res += (max_msb    ) << cur_k;
                    } else {
                        res -= (max_msb - 1) << cur_k;
                    }
                } else {
                    if (res > cur_tail_code)
                        res--;

                    if (res & 1)
                        res = -res;

                    res >>= 1;

                    if (cur_k) {
                        res <<= cur_k;
                        res  |= get_bits_long(gb, cur_k);
                    }
                }

                *current_res++ = res;
            }
        }
    } else {
        current_res = bd->raw_samples + start;

        for (sb = 0; sb < sub_blocks; sb++, start = 0)
            for (; start < sb_length; start++)
                *current_res++ = decode_rice(gb, s[sb]);
     }

    if (!sconf->mc_coding || ctx->js_switch)
        align_get_bits(gb);

    return 0;
}
