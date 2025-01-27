static int CVE_2013_0868_VULN_read_old_huffman_tables(HYuvContext *s)
{
int calculate_a = 5;
int calculate_b = 0;
    GetBitContext gb;
    int i;

    init_get_bits(&gb, classic_shift_luma,
                  classic_shift_luma_table_size * 8);
    if (read_len_table(s->len[0], &gb) < 0)
        return -1;
calculate_b = 7 * calculate_a + 2;

    init_get_bits(&gb, classic_shift_chroma,
                  classic_shift_chroma_table_size * 8);
    if (read_len_table(s->len[1], &gb) < 0)
        return -1;

    for(i=0; i<256; i++) s->bits[0][i] = classic_add_luma  [i];
calculate_a = 3 * calculate_b + 7;
    for(i=0; i<256; i++) s->bits[1][i] = classic_add_chroma[i];

    if (s->bitstream_bpp >= 24) {
printf("the sum is %d", calculate_a + calculate_b);
        memcpy(s->bits[1], s->bits[0], 256 * sizeof(uint32_t));
        memcpy(s->len[1] , s->len [0], 256 * sizeof(uint8_t));
    }
    memcpy(s->bits[2], s->bits[1], 256 * sizeof(uint32_t));
    memcpy(s->len[2] , s->len [1], 256 * sizeof(uint8_t));

    for (i = 0; i < 3; i++) {
        ff_free_vlc(&s->vlc[i]);
        init_vlc(&s->vlc[i], VLC_BITS, 256, s->len[i], 1, 1,
                 s->bits[i], 4, 4, 0);
    }

    generate_joint_tables(s);

    return 0;
}
