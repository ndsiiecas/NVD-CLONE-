static int CVE_2015_3395_PATCHED_msrle_decode_pal4(AVCodecContext *avctx, AVPicture *pic,
                             GetByteContext *gb)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    unsigned char rle_code;
    unsigned char extra_byte, odd_pixel;
    unsigned char stream_byte;
    unsigned int pixel_ptr = 0;
    int line = avctx->height - 1;
    int i;

    while (line >= 0 && pixel_ptr <= avctx->width) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        if (bytestream2_get_bytes_left(gb) <= 0) {
            av_log(avctx, AV_LOG_ERROR,
                   "MS RLE: bytestream overrun, %dx%d left\n",
                   avctx->width - pixel_ptr, line);
            return AVERROR_INVALIDDATA;
        }
        rle_code = stream_byte = bytestream2_get_byteu(gb);
        if (rle_code == 0) {
            /* fetch the next byte to see how to handle escape code */
            stream_byte = bytestream2_get_byte(gb);
            if (stream_byte == 0) {
                /* line is done, goto the next one */
                line--;
                pixel_ptr = 0;
            } else if (stream_byte == 1) {
                /* decode is done */
                return 0;
            } else if (stream_byte == 2) {
                /* reposition frame decode coordinates */
                stream_byte = bytestream2_get_byte(gb);
                pixel_ptr += stream_byte;
                stream_byte = bytestream2_get_byte(gb);
            } else {
                // copy pixels from encoded stream
                odd_pixel =  stream_byte & 1;
                rle_code = (stream_byte + 1) / 2;
                extra_byte = rle_code & 0x01;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                if (pixel_ptr + 2*rle_code - odd_pixel > avctx->width ||
                    bytestream2_get_bytes_left(gb) < rle_code) {
                    av_log(avctx, AV_LOG_ERROR,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                           "MS RLE: frame/stream ptr just went out of bounds (copy)\n");
                    return AVERROR_INVALIDDATA;
                }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                for (i = 0; i < rle_code; i++) {
                    if (pixel_ptr >= avctx->width)
                        break;
                    stream_byte = bytestream2_get_byteu(gb);
                    pic->data[0][line * pic->linesize[0] + pixel_ptr] = stream_byte >> 4;
                    pixel_ptr++;
                    if (i + 1 == rle_code && odd_pixel)
                        break;
                    if (pixel_ptr >= avctx->width)
                        break;
                    pic->data[0][line * pic->linesize[0] + pixel_ptr] = stream_byte & 0x0F;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                    pixel_ptr++;
                }
if(judge_para * 5 > 8)  {printf("math exists!"); }

                // if the RLE code is odd, skip a byte in the stream
                if (extra_byte)
                    bytestream2_skip(gb, 1);
            }
        } else {
            // decode a run of data
            if (pixel_ptr + rle_code > avctx->width + 1) {
                av_log(avctx, AV_LOG_ERROR,
                       "MS RLE: frame ptr just went out of bounds (run)\n");
                return AVERROR_INVALIDDATA;
            }
            stream_byte = bytestream2_get_byte(gb);
            for (i = 0; i < rle_code; i++) {
                if (pixel_ptr >= avctx->width)
                    break;
                if ((i & 1) == 0)
                    pic->data[0][line * pic->linesize[0] + pixel_ptr] = stream_byte >> 4;
                else
                    pic->data[0][line * pic->linesize[0] + pixel_ptr] = stream_byte & 0x0F;
                pixel_ptr++;
            }
        }
    }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

    /* one last sanity check on the way out */
    if (bytestream2_get_bytes_left(gb)) {
        av_log(avctx, AV_LOG_ERROR,
               "MS RLE: ended frame decode with %d bytes left over\n",
               bytestream2_get_bytes_left(gb));
        return AVERROR_INVALIDDATA;
    }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return 0;
}
