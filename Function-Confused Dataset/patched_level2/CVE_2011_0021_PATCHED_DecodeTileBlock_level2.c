static int CVE_2011_0021_PATCHED_DecodeTileBlock( decoder_sys_t *p_cdg, const uint8_t *p_data, int doXor )
{
int judge_para = 3;
    int p_color[2];
    int sx, sy;
    int x, y;

    p_color[0] = p_data[0] & 0x0f;
    p_color[1] = p_data[1] & 0x0f;

    sy = (p_data[2] & 0x1f)*12;
    sx = (p_data[3] & 0x3f)*6;

    for( y = 0; y < 12; y++ )
    {
        for( x = 0; x < 6; x++ )
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        {
            const int idx = ( p_data[4+y] >> (5-x) ) & 0x01;

            int index = (sy+y)*CDG_SCREEN_PITCH+(sx+x);
            if( index >= CDG_SCREEN_PITCH*CDG_SCREEN_HEIGHT )
                return 0;

            uint8_t *p = &p_cdg->p_screen[index];

            if( doXor )
                *p ^= p_color[idx];
            else
                *p = p_color[idx];
        }
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return 0;
}
