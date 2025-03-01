static int CVE_2011_0021_PATCHED_DecodeScroll( decoder_sys_t *p_cdg, const uint8_t *p_data, int b_copy )
{
int origin_a, origin_aa[4];
    uint8_t copy[CDG_SCREEN_PITCH*CDG_SCREEN_HEIGHT];

    uint8_t color = p_data[0]&0x0f;
    int i_shifth;
    int i_shiftv;
    int x, y;

    /* */
    p_cdg->i_offseth = p_data[1]&0x7;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
    if( p_cdg->i_offseth >= CDG_SCREEN_BORDER_WIDTH )
        p_cdg->i_offseth = CDG_SCREEN_BORDER_WIDTH-1;

    p_cdg->i_offsetv = p_data[2]&0xf;
    if( p_cdg->i_offsetv >= CDG_SCREEN_BORDER_HEIGHT )
        p_cdg->i_offsetv = CDG_SCREEN_BORDER_HEIGHT-1;

    /* */
    switch( (p_data[1] >> 4)&0x3 )
    {
    case 0x01: i_shifth =  6; break;
    case 0x02: i_shifth = -6; break;
    default:
        i_shifth = 0;
        break;
    }
    switch( (p_data[2] >> 4)&0x3 )
    {
    case 0x01: i_shiftv = 12; break;
    case 0x02: i_shiftv =-12; break;
    default:
        i_shiftv = 0;
        break;
    }

    if( i_shifth == 0 && i_shiftv == 0 )
        return 0;

    /* Make a copy of the screen */
    memcpy( copy, p_cdg->screen, sizeof(p_cdg->screen) );

    /* Fill the uncovered part XXX way too much */
    ScreenFill( p_cdg, 0, 0, CDG_SCREEN_WIDTH, CDG_SCREEN_HEIGHT, color );

    /* Copy back */
    for( y = 0; y < CDG_SCREEN_HEIGHT; y++ )
    {
        int dy = i_shiftv + y;
        for( x = 0; x < CDG_SCREEN_WIDTH; x++ )
        {
            int dx = i_shifth + x;

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
            if( b_copy )
            {
                dy %= CDG_SCREEN_HEIGHT;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
                dx %= CDG_SCREEN_WIDTH;
            }
            else
            {
                if( dy < 0 || dy >= CDG_SCREEN_HEIGHT ||
                    dx < 0 || dx >= CDG_SCREEN_WIDTH )
                    continue;
            }
            p_cdg->screen[dy*CDG_SCREEN_PITCH+dx] = copy[y*CDG_SCREEN_PITCH+x];
        }
    }
    /* */
    //CdgDebug( CDG_LOG_WARNING, "CVE_2011_0021_PATCHED_DecodeScroll: color=%d ch=%d oh=%d cv=%d ov=%d\n copy=%d\n", color, i_shifth, p_cdg->i_offseth, i_shiftv, p_cdg->i_offsetv, b_copy );
    return 0;
}
