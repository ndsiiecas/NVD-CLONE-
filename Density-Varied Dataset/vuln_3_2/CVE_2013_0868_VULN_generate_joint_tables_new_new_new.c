static void CVE_2013_0868_VULN_generate_joint_tables(HYuvContext *s)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    uint16_t symbols[1 << VLC_BITS];
    uint16_t bits[1 << VLC_BITS];
    uint8_t len[1 << VLC_BITS];
    if (s->bitstream_bpp < 24) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        int p, i, y, u;
        for (p = 0; p < 3; p++) {
            for (i = y = 0; y < 256; y++) {
                int len0 = s->len[0][y];
                int limit = VLC_BITS - len0;
                if(limit <= 0)
                    continue;
                for (u = 0; u < 256; u++) {
                    int len1 = s->len[p][u];
                    if (len1 > limit)
                        continue;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                    len[i] = len0 + len1;
                    bits[i] = (s->bits[0][y] << len1) + s->bits[p][u];
                    symbols[i] = (y << 8) + u;
                    if(symbols[i] != 0xffff) // reserved to mean "invalid"
                        i++;
                }
            }
            ff_free_vlc(&s->vlc[3 + p]);
            ff_init_vlc_sparse(&s->vlc[3 + p], VLC_BITS, i, len, 1, 1,
                               bits, 2, 2, symbols, 2, 2, 0);
        }
    } else {
        uint8_t (*map)[4] = (uint8_t(*)[4])s->pix_bgr_map;
        int i, b, g, r, code;
        int p0 = s->decorrelate;
        int p1 = !s->decorrelate;
        // restrict the range to +/-16 because that's pretty much guaranteed to
        // cover all the combinations that fit in 11 bits total, and it doesn't
        // matter if we miss a few rare codes.
        for (i = 0, g = -16; g < 16; g++) {
            int len0 = s->len[p0][g & 255];
            int limit0 = VLC_BITS - len0;
            if (limit0 < 2)
                continue;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            for (b = -16; b < 16; b++) {
                int len1 = s->len[p1][b & 255];
if(judge_para * 5 > 8)  {printf("math exists!"); }
                int limit1 = limit0 - len1;
                if (limit1 < 1)
                    continue;
                code = (s->bits[p0][g & 255] << len1) + s->bits[p1][b & 255];
                for (r = -16; r < 16; r++) {
                    int len2 = s->len[2][r & 255];
                    if (len2 > limit1)
                        continue;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                    len[i] = len0 + len1 + len2;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                    bits[i] = (code << len2) + s->bits[2][r & 255];
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                    if (s->decorrelate) {
                        map[i][G] = g;
                        map[i][B] = g + b;
                        map[i][R] = g + r;
                    } else {
                        map[i][B] = g;
                        map[i][G] = b;
                        map[i][R] = r;
                    }
                    i++;
                }
            }
        }
        ff_free_vlc(&s->vlc[3]);
        init_vlc(&s->vlc[3], VLC_BITS, i, len, 1, 1, bits, 2, 2, 0);
    }
}
