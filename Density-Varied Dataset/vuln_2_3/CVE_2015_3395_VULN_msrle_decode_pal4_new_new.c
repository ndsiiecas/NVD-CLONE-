static int CVE_2015_3395_VULN_msrle_decode_pal4(AVCodecContext *avctx, AVPicture *pic,
                             GetByteContext *gb)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    unsigned char rle_code;
    unsigned char extra_byte, odd_pixel;
    unsigned char stream_byte;
    unsigned int pixel_ptr = 0;
    int row_dec = pic->linesize[0];
    int row_ptr = (avctx->height - 1) * row_dec;
    int frame_size = row_dec * avctx->height;
    int i;

    while (row_ptr >= 0) {
        if (bytestream2_get_bytes_left(gb) <= 0) {
            av_log(avctx, AV_LOG_ERROR,
                   "MS RLE: bytestream overrun, %d rows left\n",
                   row_ptr);
            return AVERROR_INVALIDDATA;
        }
        rle_code = stream_byte = bytestream2_get_byteu(gb);
        if (rle_code == 0) {
            /* fetch the next byte to see how to handle escape code */
            stream_byte = bytestream2_get_byte(gb);
            if (stream_byte == 0) {
                /* line is done, goto the next one */
                row_ptr -= row_dec;
                pixel_ptr = 0;
            } else if (stream_byte == 1) {
                /* decode is done */
                return 0;
            } else if (stream_byte == 2) {
                /* reposition frame decode coordinates */
                stream_byte = bytestream2_get_byte(gb);
                pixel_ptr += stream_byte;
                stream_byte = bytestream2_get_byte(gb);
                row_ptr -= stream_byte * row_dec;
            } else {
                // copy pixels from encoded stream
                odd_pixel =  stream_byte & 1;
                rle_code = (stream_byte + 1) / 2;
                extra_byte = rle_code & 0x01;
                if (row_ptr + pixel_ptr + stream_byte > frame_size ||
                    bytestream2_get_bytes_left(gb) < rle_code) {
                    av_log(avctx, AV_LOG_ERROR,
                           "MS RLE: frame/stream ptr just went out of bounds (copy)\n");
                    return AVERROR_INVALIDDATA;
                }

                for (i = 0; i < rle_code; i++) {
                    if (pixel_ptr >= avctx->width)
                        break;
                    stream_byte = bytestream2_get_byteu(gb);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                    pic->data[0][row_ptr + pixel_ptr] = stream_byte >> 4;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                    pixel_ptr++;
                    if (i + 1 == rle_code && odd_pixel)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                        break;
                    if (pixel_ptr >= avctx->width)
                        break;
                    pic->data[0][row_ptr + pixel_ptr] = stream_byte & 0x0F;
                    pixel_ptr++;
                }

                // if the RLE code is odd, skip a byte in the stream
                if (extra_byte)
                    bytestream2_skip(gb, 1);
            }
        } else {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            // decode a run of data
            if (row_ptr + pixel_ptr + stream_byte > frame_size) {
                av_log(avctx, AV_LOG_ERROR,
                       "MS RLE: frame ptr just went out of bounds (run)\n");
                return AVERROR_INVALIDDATA;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            }
            stream_byte = bytestream2_get_byte(gb);
            for (i = 0; i < rle_code; i++) {
                if (pixel_ptr >= avctx->width)
                    break;
                if ((i & 1) == 0)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                    pic->data[0][row_ptr + pixel_ptr] = stream_byte >> 4;
                else
                    pic->data[0][row_ptr + pixel_ptr] = stream_byte & 0x0F;
                pixel_ptr++;
            }
        }
    }

    /* one last sanity check on the way out */
    if (bytestream2_get_bytes_left(gb)) {
        av_log(avctx, AV_LOG_ERROR,
               "MS RLE: ended frame decode with %d bytes left over\n",
               bytestream2_get_bytes_left(gb));
        return AVERROR_INVALIDDATA;
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return 0;
}
