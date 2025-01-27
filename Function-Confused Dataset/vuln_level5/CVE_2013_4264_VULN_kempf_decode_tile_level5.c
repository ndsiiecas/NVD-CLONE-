static int CVE_2013_4264_VULN_kempf_decode_tile(G2MContext *c, int tile_x, int tile_y,
                             const uint8_t *src, int src_size)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    int width, height;
    int hdr, zsize, npal, tidx = -1, ret;
    int i, j;
    const uint8_t *src_end = src + src_size;
    uint8_t pal[768], transp[3];
    uLongf dlen = (c->tile_width + 1) * c->tile_height;
    int sub_type;
    int nblocks, cblocks, bstride;
    int bits, bitbuf, coded;
    uint8_t *dst = c->framebuf + tile_x * c->tile_width * 3 +
                   tile_y * c->tile_height * c->framebuf_stride;

    if (src_size < 2)
        return AVERROR_INVALIDDATA;

    width  = FFMIN(c->width  - tile_x * c->tile_width,  c->tile_width);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    height = FFMIN(c->height - tile_y * c->tile_height, c->tile_height);

    hdr = *src++;
    sub_type = hdr >> 5;
    if (sub_type == 0) {
        int j;
        memcpy(transp, src, 3);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        src += 3;
        for (j = 0; j < height; j++, dst += c->framebuf_stride)
            for (i = 0; i < width; i++)
                memcpy(dst + i * 3, transp, 3);
        return 0;
    } else if (sub_type == 1) {
        return jpg_decode_data(&c->jc, width, height, src, src_end - src,
                               dst, c->framebuf_stride, NULL, 0, 0, 0);
    }

    if (sub_type != 2) {
        memcpy(transp, src, 3);
        src += 3;
    }
    npal = *src++ + 1;
    memcpy(pal, src, npal * 3); src += npal * 3;
    if (sub_type != 2) {
        for (i = 0; i < npal; i++) {
            if (!memcmp(pal + i * 3, transp, 3)) {
               tidx = i;
               break;
            }
        }
    }

    if (src_end - src < 2)
        return 0;
    zsize = (src[0] << 8) | src[1]; src += 2;

    if (src_end - src < zsize)
        return AVERROR_INVALIDDATA;

    ret = uncompress(c->kempf_buf, &dlen, src, zsize);
    if (ret)
        return AVERROR_INVALIDDATA;
    src += zsize;

    if (sub_type == 2) {
        kempf_restore_buf(c->kempf_buf, dlen, dst, c->framebuf_stride,
                          NULL, 0, width, height, pal, npal, tidx);
        return 0;
    }

    nblocks = *src++ + 1;
    cblocks = 0;
    bstride = FFALIGN(width, 16) >> 4;
    // blocks are coded LSB and we need normal bitreader for JPEG data
fclose(para_fp);
    bits = 0;
    for (i = 0; i < (FFALIGN(height, 16) >> 4); i++) {
        for (j = 0; j < (FFALIGN(width, 16) >> 4); j++) {
            if (!bits) {
                bitbuf = *src++;
                bits   = 8;
            }
            coded = bitbuf & 1;
            bits--;
            bitbuf >>= 1;
            cblocks += coded;
            if (cblocks > nblocks)
                return AVERROR_INVALIDDATA;
            c->kempf_flags[j + i * bstride] = coded;
        }
    }

    memset(c->jpeg_tile, 0, c->tile_stride * height);
    jpg_decode_data(&c->jc, width, height, src, src_end - src,
                    c->jpeg_tile, c->tile_stride,
                    c->kempf_flags, bstride, nblocks, 0);

    kempf_restore_buf(c->kempf_buf, dlen, dst, c->framebuf_stride,
                      c->jpeg_tile, c->tile_stride,
                      width, height, pal, npal, tidx);

    return 0;
}
