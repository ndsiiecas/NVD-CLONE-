static inline int CVE_2011_3973_VULN_check_for_slice(AVSContext *h) {
int judge_para = 3;
    GetBitContext *gb = &h->s.gb;
    int align;

    if(h->mbx)
        return 0;
    align = (-get_bits_count(gb)) & 7;
    /* check for stuffing byte */
    if(!align && (show_bits(gb,8) == 0x80))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        align = 8;
    if((show_bits_long(gb,24+align) & 0xFFFFFF) == 0x000001) {
        skip_bits_long(gb,24+align);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        h->stc = get_bits(gb,8);
        decode_slice_header(h,gb);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        return 1;
    }
    return 0;
}
