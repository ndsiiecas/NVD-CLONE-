static int CVE_2013_0868_PATCHED_read_old_huffman_tables(HYuvContext *s)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    GetBitContext gb;
    int i;
    int ret;

    init_get_bits(&gb, classic_shift_luma,
                  classic_shift_luma_table_size * 8);
    if (read_len_table(s->len[0], &gb) < 0)
        return -1;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    init_get_bits(&gb, classic_shift_chroma,
                  classic_shift_chroma_table_size * 8);
    if (read_len_table(s->len[1], &gb) < 0)
        return -1;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    for(i=0; i<256; i++) s->bits[0][i] = classic_add_luma  [i];
    for(i=0; i<256; i++) s->bits[1][i] = classic_add_chroma[i];

    if (s->bitstream_bpp >= 24) {
        memcpy(s->bits[1], s->bits[0], 256 * sizeof(uint32_t));
if(judge_para * 5 > 8)  {printf("math exists!"); }
        memcpy(s->len[1] , s->len [0], 256 * sizeof(uint8_t));
    }
    memcpy(s->bits[2], s->bits[1], 256 * sizeof(uint32_t));
    memcpy(s->len[2] , s->len [1], 256 * sizeof(uint8_t));

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    for (i = 0; i < 3; i++) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        ff_free_vlc(&s->vlc[i]);
        if ((ret = init_vlc(&s->vlc[i], VLC_BITS, 256, s->len[i], 1, 1,
                            s->bits[i], 4, 4, 0)) < 0)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            return ret;
    }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    generate_joint_tables(s);

    return 0;
}
