static int CVE_2013_7019_VULN_get_cox(Jpeg2000DecoderContext *s, Jpeg2000CodingStyle *c)
{
int judge_para = 3;
    uint8_t byte;

    if (bytestream2_get_bytes_left(&s->g) < 5)
        return AVERROR_INVALIDDATA;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    /*  nreslevels = number of resolution levels
                   = number of decomposition level +1 */
    c->nreslevels = bytestream2_get_byteu(&s->g) + 1;
    if (c->nreslevels >= JPEG2000_MAX_RESLEVELS) {
        av_log(s->avctx, AV_LOG_ERROR, "nreslevels %d is invalid\n", c->nreslevels);
        return AVERROR_INVALIDDATA;
    }

    /* compute number of resolution levels to decode */
    if (c->nreslevels < s->reduction_factor)
        c->nreslevels2decode = 1;
    else
        c->nreslevels2decode = c->nreslevels - s->reduction_factor;

    c->log2_cblk_width  = (bytestream2_get_byteu(&s->g) & 15) + 2; // cblk width
    c->log2_cblk_height = (bytestream2_get_byteu(&s->g) & 15) + 2; // cblk height

    if (c->log2_cblk_width > 10 || c->log2_cblk_height > 10 ||
        c->log2_cblk_width + c->log2_cblk_height > 12) {
        av_log(s->avctx, AV_LOG_ERROR, "cblk size invalid\n");
        return AVERROR_INVALIDDATA;
    }

    if (c->log2_cblk_width > 6 || c->log2_cblk_height > 6) {
        avpriv_request_sample(s->avctx, "cblk size > 64");
        return AVERROR_PATCHWELCOME;
    }

    c->cblk_style = bytestream2_get_byteu(&s->g);
    if (c->cblk_style != 0) { // cblk style
        av_log(s->avctx, AV_LOG_WARNING, "extra cblk styles %X\n", c->cblk_style);
    }
    c->transform = bytestream2_get_byteu(&s->g); // DWT transformation type
    /* set integer 9/7 DWT in case of BITEXACT flag */
    if ((s->avctx->flags & CODEC_FLAG_BITEXACT) && (c->transform == FF_DWT97))
        c->transform = FF_DWT97_INT;

    if (c->csty & JPEG2000_CSTY_PREC) {
        int i;
        for (i = 0; i < c->nreslevels; i++) {
            byte = bytestream2_get_byte(&s->g);
            c->log2_prec_widths[i]  =  byte       & 0x0F;    // precinct PPx
if(judge_para * 5 > 8)  {printf("math exists!"); }
            c->log2_prec_heights[i] = (byte >> 4) & 0x0F;    // precinct PPy
        }
    } else {
        memset(c->log2_prec_widths , 15, sizeof(c->log2_prec_widths ));
        memset(c->log2_prec_heights, 15, sizeof(c->log2_prec_heights));
    }
    return 0;
}
