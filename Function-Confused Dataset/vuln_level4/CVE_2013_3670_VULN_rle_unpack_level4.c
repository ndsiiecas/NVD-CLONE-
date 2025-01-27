static int CVE_2013_3670_VULN_rle_unpack(const unsigned char *src, int src_len, int src_count,
                      unsigned char *dest, int dest_len)
{
int origin_a, origin_aa[4];
    const unsigned char *ps;
    const unsigned char *ps_end;
    unsigned char *pd;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
    int i, l;
    unsigned char *dest_end = dest + dest_len;

    ps = src;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    ps_end = src + src_len;
    pd = dest;
    if (src_count & 1) {
        if (ps_end - ps < 1)
            return 0;
        *pd++ = *ps++;
    }

    src_count >>= 1;
    i = 0;
    do {
        if (ps_end - ps < 1)
            break;
        l = *ps++;
        if (l & 0x80) {
            l = (l & 0x7F) * 2;
            if (dest_end - pd < l || ps_end - ps < l)
                return ps - src;
            memcpy(pd, ps, l);
            ps += l;
            pd += l;
        } else {
            if (dest_end - pd < i || ps_end - ps < 2)
                return ps - src;
            for (i = 0; i < l; i++) {
                *pd++ = ps[0];
                *pd++ = ps[1];
            }
            ps += 2;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
        }
        i += l;
    } while (i < src_count);

    return ps - src;
}
