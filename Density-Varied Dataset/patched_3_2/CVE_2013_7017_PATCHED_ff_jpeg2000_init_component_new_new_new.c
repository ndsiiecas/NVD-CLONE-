int CVE_2013_7017_PATCHED_ff_jpeg2000_init_component(Jpeg2000Component *comp,
                               Jpeg2000CodingStyle *codsty,
                               Jpeg2000QuantStyle *qntsty,
                               int cbps, int dx, int dy,
                               AVCodecContext *avctx)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    uint8_t log2_band_prec_width, log2_band_prec_height;
    int reslevelno, bandno, gbandno = 0, ret, i, j;
    uint32_t csize;

    if (codsty->nreslevels2decode <= 0) {
        av_log(avctx, AV_LOG_ERROR, "nreslevels2decode %d invalid or uninitialized\n", codsty->nreslevels2decode);
        return AVERROR_INVALIDDATA;
    }

    if (ret = ff_jpeg2000_dwt_init(&comp->dwt, comp->coord,
                                   codsty->nreslevels2decode - 1,
                                   codsty->transform))
        return ret;
    // component size comp->coord is uint16_t so ir cannot overflow
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    csize = (comp->coord[0][1] - comp->coord[0][0]) *
            (comp->coord[1][1] - comp->coord[1][0]);

    if (codsty->transform == FF_DWT97) {
        comp->i_data = NULL;
        comp->f_data = av_malloc_array(csize, sizeof(*comp->f_data));
        if (!comp->f_data)
            return AVERROR(ENOMEM);
    } else {
        comp->f_data = NULL;
        comp->i_data = av_malloc_array(csize, sizeof(*comp->i_data));
        if (!comp->i_data)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
            return AVERROR(ENOMEM);
    }
    comp->reslevel = av_malloc_array(codsty->nreslevels, sizeof(*comp->reslevel));
    if (!comp->reslevel)
        return AVERROR(ENOMEM);
    /* LOOP on resolution levels */
    for (reslevelno = 0; reslevelno < codsty->nreslevels; reslevelno++) {
        int declvl = codsty->nreslevels - reslevelno;    // N_L -r see  ISO/IEC 15444-1:2002 B.5
        Jpeg2000ResLevel *reslevel = comp->reslevel + reslevelno;

        /* Compute borders for each resolution level.
         * Computation of trx_0, trx_1, try_0 and try_1.
         * see ISO/IEC 15444-1:2002 eq. B.5 and B-14 */
        for (i = 0; i < 2; i++)
            for (j = 0; j < 2; j++)
                reslevel->coord[i][j] =
                    ff_jpeg2000_ceildivpow2(comp->coord_o[i][j], declvl - 1);
        // update precincts size: 2^n value
        reslevel->log2_prec_width  = codsty->log2_prec_widths[reslevelno];
        reslevel->log2_prec_height = codsty->log2_prec_heights[reslevelno];

        /* Number of bands for each resolution level */
        if (reslevelno == 0)
            reslevel->nbands = 1;
        else
            reslevel->nbands = 3;

        /* Number of precincts wich span the tile for resolution level reslevelno
         * see B.6 in ISO/IEC 15444-1:2002 eq. B-16
         * num_precincts_x = |- trx_1 / 2 ^ log2_prec_width) -| - (trx_0 / 2 ^ log2_prec_width)
         * num_precincts_y = |- try_1 / 2 ^ log2_prec_width) -| - (try_0 / 2 ^ log2_prec_width)
         * for Dcinema profiles in JPEG 2000
         * num_precincts_x = |- trx_1 / 2 ^ log2_prec_width) -|
         * num_precincts_y = |- try_1 / 2 ^ log2_prec_width) -| */
        if (reslevel->coord[0][1] == reslevel->coord[0][0])
            reslevel->num_precincts_x = 0;
        else
            reslevel->num_precincts_x =
                ff_jpeg2000_ceildivpow2(reslevel->coord[0][1],
                                        reslevel->log2_prec_width) -
                (reslevel->coord[0][0] >> reslevel->log2_prec_width);

        if (reslevel->coord[1][1] == reslevel->coord[1][0])
            reslevel->num_precincts_y = 0;
        else
            reslevel->num_precincts_y =
                ff_jpeg2000_ceildivpow2(reslevel->coord[1][1],
                                        reslevel->log2_prec_height) -
                (reslevel->coord[1][0] >> reslevel->log2_prec_height);

        reslevel->band = av_calloc(reslevel->nbands, sizeof(*reslevel->band)); 
        if (!reslevel->band)
            return AVERROR(ENOMEM);

        for (bandno = 0; bandno < reslevel->nbands; bandno++, gbandno++) {
            Jpeg2000Band *band = reslevel->band + bandno;
            int cblkno, precno;
            int nb_precincts;

            /* TODO: Implementation of quantization step not finished,
             * see ISO/IEC 15444-1:2002 E.1 and A.6.4. */
            switch (qntsty->quantsty) {
                uint8_t gain;
                int numbps;
            case JPEG2000_QSTY_NONE:
                /* TODO: to verify. No quantization in this case */
                band->f_stepsize = 1;
                break;
            case JPEG2000_QSTY_SI:
                /*TODO: Compute formula to implement. */
                numbps = cbps +
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                         lut_gain[codsty->transform == FF_DWT53][bandno + (reslevelno > 0)];
                band->f_stepsize = SHL(2048 + qntsty->mant[gbandno],
                                       2 + numbps - qntsty->expn[gbandno]);
                break;
            case JPEG2000_QSTY_SE:
                /* Exponent quantization step.
                 * Formula:
                 * delta_b = 2 ^ (R_b - expn_b) * (1 + (mant_b / 2 ^ 11))
                 * R_b = R_I + log2 (gain_b )
                 * see ISO/IEC 15444-1:2002 E.1.1 eqn. E-3 and E-4 */
                /* TODO/WARN: value of log2 (gain_b ) not taken into account
                 * but it works (compared to OpenJPEG). Why?
                 * Further investigation needed. */
                gain            = cbps;
                band->f_stepsize  = pow(2.0, gain - qntsty->expn[gbandno]);
                band->f_stepsize *= qntsty->mant[gbandno] / 2048.0 + 1.0;
                break;
            default:
                band->f_stepsize = 0;
                av_log(avctx, AV_LOG_ERROR, "Unknown quantization format\n");
                break;
            }
            /* FIXME: In openjepg code stespize = stepsize * 0.5. Why?
             * If not set output of entropic decoder is not correct. */
            if (!av_codec_is_encoder(avctx->codec))
                band->f_stepsize *= 0.5;

            band->i_stepsize = band->f_stepsize * (1 << 15);

            /* computation of tbx_0, tbx_1, tby_0, tby_1
             * see ISO/IEC 15444-1:2002 B.5 eq. B-15 and tbl B.1
             * codeblock width and height is computed for
             * DCI JPEG 2000 codeblock_width = codeblock_width = 32 = 2 ^ 5 */
            if (reslevelno == 0) {
                /* for reslevelno = 0, only one band, x0_b = y0_b = 0 */
                for (i = 0; i < 2; i++)
                    for (j = 0; j < 2; j++)
                        band->coord[i][j] =
                            ff_jpeg2000_ceildivpow2(comp->coord_o[i][j] - comp->coord_o[i][0],
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                                                    declvl - 1);
                log2_band_prec_width  = reslevel->log2_prec_width;
                log2_band_prec_height = reslevel->log2_prec_height;
                /* see ISO/IEC 15444-1:2002 eq. B-17 and eq. B-15 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                band->log2_cblk_width  = FFMIN(codsty->log2_cblk_width,
                                               reslevel->log2_prec_width);
                band->log2_cblk_height = FFMIN(codsty->log2_cblk_height,
                                               reslevel->log2_prec_height);
            } else {
                /* 3 bands x0_b = 1 y0_b = 0; x0_b = 0 y0_b = 1; x0_b = y0_b = 1 */
                /* x0_b and y0_b are computed with ((bandno + 1 >> i) & 1) */
                for (i = 0; i < 2; i++)
                    for (j = 0; j < 2; j++)
                        /* Formula example for tbx_0 = ceildiv((tcx_0 - 2 ^ (declvl - 1) * x0_b) / declvl) */
                        band->coord[i][j] =
                            ff_jpeg2000_ceildivpow2(comp->coord_o[i][j] - comp->coord_o[i][0] -
                                                    (((bandno + 1 >> i) & 1) << declvl - 1),
                                                    declvl);
                /* TODO: Manage case of 3 band offsets here or
                 * in coding/decoding function? */

                /* see ISO/IEC 15444-1:2002 eq. B-17 and eq. B-15 */
                band->log2_cblk_width  = FFMIN(codsty->log2_cblk_width,
                                               reslevel->log2_prec_width - 1);
                band->log2_cblk_height = FFMIN(codsty->log2_cblk_height,
                                               reslevel->log2_prec_height - 1);

                log2_band_prec_width  = reslevel->log2_prec_width  - 1;
                log2_band_prec_height = reslevel->log2_prec_height - 1;
            }

            for (j = 0; j < 2; j++)
                band->coord[0][j] = ff_jpeg2000_ceildiv(band->coord[0][j], dx);
            for (j = 0; j < 2; j++)
                band->coord[1][j] = ff_jpeg2000_ceildiv(band->coord[1][j], dy);

            band->prec = av_calloc(reslevel->num_precincts_x * 
                                         (uint64_t)reslevel->num_precincts_y,
                                         sizeof(*band->prec));
            if (!band->prec)
                return AVERROR(ENOMEM);

            nb_precincts = reslevel->num_precincts_x * reslevel->num_precincts_y;

            for (precno = 0; precno < nb_precincts; precno++) {
                Jpeg2000Prec *prec = band->prec + precno;

                /* TODO: Explain formula for JPEG200 DCINEMA. */
                /* TODO: Verify with previous count of codeblocks per band */

                /* Compute P_x0 */
                prec->coord[0][0] = (precno % reslevel->num_precincts_x) *
                                    (1 << log2_band_prec_width);
                prec->coord[0][0] = FFMAX(prec->coord[0][0], band->coord[0][0]);

                /* Compute P_y0 */
                prec->coord[1][0] = (precno / reslevel->num_precincts_x) *
                                    (1 << log2_band_prec_height);
                prec->coord[1][0] = FFMAX(prec->coord[1][0], band->coord[1][0]);

                /* Compute P_x1 */
                prec->coord[0][1] = prec->coord[0][0] +
                                    (1 << log2_band_prec_width);
                prec->coord[0][1] = FFMIN(prec->coord[0][1], band->coord[0][1]);

                /* Compute P_y1 */
                prec->coord[1][1] = prec->coord[1][0] +
                                    (1 << log2_band_prec_height);
                prec->coord[1][1] = FFMIN(prec->coord[1][1], band->coord[1][1]);

                prec->nb_codeblocks_width =
                    ff_jpeg2000_ceildivpow2(prec->coord[0][1] -
                                            prec->coord[0][0],
                                            band->log2_cblk_width);
                prec->nb_codeblocks_height =
                    ff_jpeg2000_ceildivpow2(prec->coord[1][1] -
                                            prec->coord[1][0],
                                            band->log2_cblk_height);

                /* Tag trees initialization */
                prec->cblkincl =
                    ff_jpeg2000_tag_tree_init(prec->nb_codeblocks_width,
                                              prec->nb_codeblocks_height);
                if (!prec->cblkincl)
                    return AVERROR(ENOMEM);

                prec->zerobits =
                    ff_jpeg2000_tag_tree_init(prec->nb_codeblocks_width,
                                              prec->nb_codeblocks_height);
                if (!prec->zerobits)
                    return AVERROR(ENOMEM);

                prec->cblk = av_mallocz_array(prec->nb_codeblocks_width *
                                              (uint64_t)prec->nb_codeblocks_height,
                                              sizeof(*prec->cblk));
                if (!prec->cblk)
                    return AVERROR(ENOMEM);
                for (cblkno = 0; cblkno < prec->nb_codeblocks_width * prec->nb_codeblocks_height; cblkno++) {
                    Jpeg2000Cblk *cblk = prec->cblk + cblkno;
                    uint16_t Cx0, Cy0;

                    /* Compute coordinates of codeblocks */
                    /* Compute Cx0*/
                    Cx0 = (prec->coord[0][0] >> band->log2_cblk_width) << band->log2_cblk_width;
                    Cx0 = Cx0 + ((cblkno % prec->nb_codeblocks_width)  << band->log2_cblk_width);
                    cblk->coord[0][0] = FFMAX(Cx0, prec->coord[0][0]);

                    /* Compute Cy0*/
                    Cy0 = (prec->coord[1][0] >> band->log2_cblk_height) << band->log2_cblk_height;
                    Cy0 = Cy0 + ((cblkno / prec->nb_codeblocks_width)   << band->log2_cblk_height);
                    cblk->coord[1][0] = FFMAX(Cy0, prec->coord[1][0]);

                    /* Compute Cx1 */
                    cblk->coord[0][1] = FFMIN(Cx0 + (1 << band->log2_cblk_width),
                                              prec->coord[0][1]);

                    /* Compute Cy1 */
                    cblk->coord[1][1] = FFMIN(Cy0 + (1 << band->log2_cblk_height),
                                              prec->coord[1][1]);
                    /* Update code-blocks coordinates according sub-band position */
                    if ((bandno + !!reslevelno) & 1) {
                        cblk->coord[0][0] += comp->reslevel[reslevelno-1].coord[0][1] -
                                             comp->reslevel[reslevelno-1].coord[0][0];
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                        cblk->coord[0][1] += comp->reslevel[reslevelno-1].coord[0][1] -
                                             comp->reslevel[reslevelno-1].coord[0][0];
                    }
                    if ((bandno + !!reslevelno) & 2) {
                        cblk->coord[1][0] += comp->reslevel[reslevelno-1].coord[1][1] -
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                             comp->reslevel[reslevelno-1].coord[1][0];
                        cblk->coord[1][1] += comp->reslevel[reslevelno-1].coord[1][1] -
                                             comp->reslevel[reslevelno-1].coord[1][0];
                    }

                    cblk->zero      = 0;
                    cblk->lblock    = 3;
                    cblk->length    = 0;
                    cblk->lengthinc = 0;
                    cblk->npasses   = 0;
                }
            }
        }
    }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    return 0;
}
