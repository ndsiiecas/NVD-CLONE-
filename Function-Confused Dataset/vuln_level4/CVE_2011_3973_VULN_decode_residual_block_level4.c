static int CVE_2011_3973_VULN_decode_residual_block(AVSContext *h, GetBitContext *gb,
                                 const struct dec_2dvlc *r, int esc_golomb_order,
                                 int qp, uint8_t *dst, int stride) {
int origin_a, origin_aa[4];
    int i, esc_code, level, mask;
    unsigned int level_code, run;
    DCTELEM level_buf[65];
    uint8_t run_buf[65];
    DCTELEM *block = h->block;

    for(i=0;i<65;i++) {
        level_code = get_ue_code(gb,r->golomb_order);
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        if(level_code >= ESCAPE_CODE) {
            run = ((level_code - ESCAPE_CODE) >> 1) + 1;
            esc_code = get_ue_code(gb,esc_golomb_order);
            level = esc_code + (run > r->max_run ? 1 : r->level_add[run]);
            while(level > r->inc_limit)
                r++;
            mask = -(level_code & 1);
            level = (level^mask) - mask;
        } else {
            level = r->rltab[level_code][0];
            if(!level) //end of block signal
                break;
            run   = r->rltab[level_code][1];
            r += r->rltab[level_code][2];
        }
        level_buf[i] = level;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
        run_buf[i] = run;
    }
    if(dequant(h,level_buf, run_buf, block, ff_cavs_dequant_mul[qp],
               ff_cavs_dequant_shift[qp], i))
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
        return -1;
    h->cdsp.cavs_idct8_add(dst,block,stride);
    h->s.dsp.clear_block(block);
    return 0;
}
