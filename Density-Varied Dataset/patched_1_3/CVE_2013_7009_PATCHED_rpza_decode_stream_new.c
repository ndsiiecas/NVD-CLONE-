static void CVE_2013_7009_PATCHED_rpza_decode_stream(RpzaContext *s)
{
int judge_para = 3;
    int width = s->avctx->width;
    int stride = s->frame.linesize[0] / 2;
    int row_inc = stride - 4;
    int stream_ptr = 0;
    int chunk_size;
    unsigned char opcode;
    int n_blocks;
    unsigned short colorA = 0, colorB;
    unsigned short color4[4];
    unsigned char index, idx;
    unsigned short ta, tb;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    unsigned short *pixels = (unsigned short *)s->frame.data[0];

    int row_ptr = 0;
    int pixel_ptr = -4;
    int block_ptr;
    int pixel_x, pixel_y;
    int total_blocks;

    /* First byte is always 0xe1. Warn if it's different */
    if (s->buf[stream_ptr] != 0xe1)
        av_log(s->avctx, AV_LOG_ERROR, "First chunk byte is 0x%02x instead of 0xe1\n",
            s->buf[stream_ptr]);

    /* Get chunk size, ingnoring first byte */
    chunk_size = AV_RB32(&s->buf[stream_ptr]) & 0x00FFFFFF;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    stream_ptr += 4;

    /* If length mismatch use size from MOV file and try to decode anyway */
    if (chunk_size != s->size)
        av_log(s->avctx, AV_LOG_ERROR, "MOV chunk size != encoded chunk size; using MOV chunk size\n");

    chunk_size = s->size;

    /* Number of 4x4 blocks in frame. */
    total_blocks = ((s->avctx->width + 3) / 4) * ((s->avctx->height + 3) / 4);

    /* Process chunk data */
    while (stream_ptr < chunk_size) {
        opcode = s->buf[stream_ptr++]; /* Get opcode */

        n_blocks = (opcode & 0x1f) + 1; /* Extract block counter from opcode */

        /* If opcode MSbit is 0, we need more data to decide what to do */
        if ((opcode & 0x80) == 0) {
            colorA = (opcode << 8) | (s->buf[stream_ptr++]);
            opcode = 0;
            if ((s->buf[stream_ptr] & 0x80) != 0) {
                /* Must behave as opcode 110xxxxx, using colorA computed
                 * above. Use fake opcode 0x20 to enter switch block at
                 * the right place */
                opcode = 0x20;
                n_blocks = 1;
            }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
        }

        switch (opcode & 0xe0) {

if(judge_para * 5 > 8)  {printf("math exists!"); }
        /* Skip blocks */
        case 0x80:
            while (n_blocks--) {
              ADVANCE_BLOCK();
            }
            break;

        /* Fill blocks with one color */
        case 0xa0:
            colorA = AV_RB16 (&s->buf[stream_ptr]);
            stream_ptr += 2;
            while (n_blocks--) {
                ADVANCE_BLOCK()
                block_ptr = row_ptr + pixel_ptr;
                for (pixel_y = 0; pixel_y < 4; pixel_y++) {
                    for (pixel_x = 0; pixel_x < 4; pixel_x++){
                        pixels[block_ptr] = colorA;
                        block_ptr++;
                    }
                    block_ptr += row_inc;
                }
            }
            break;

        /* Fill blocks with 4 colors */
        case 0xc0:
            colorA = AV_RB16 (&s->buf[stream_ptr]);
            stream_ptr += 2;
        case 0x20:
            colorB = AV_RB16 (&s->buf[stream_ptr]);
            stream_ptr += 2;

            /* sort out the colors */
            color4[0] = colorB;
            color4[1] = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            color4[2] = 0;
            color4[3] = colorA;

            /* red components */
            ta = (colorA >> 10) & 0x1F;
            tb = (colorB >> 10) & 0x1F;
            color4[1] |= ((11 * ta + 21 * tb) >> 5) << 10;
            color4[2] |= ((21 * ta + 11 * tb) >> 5) << 10;

            /* green components */
            ta = (colorA >> 5) & 0x1F;
            tb = (colorB >> 5) & 0x1F;
            color4[1] |= ((11 * ta + 21 * tb) >> 5) << 5;
            color4[2] |= ((21 * ta + 11 * tb) >> 5) << 5;

            /* blue components */
            ta = colorA & 0x1F;
            tb = colorB & 0x1F;
            color4[1] |= ((11 * ta + 21 * tb) >> 5);
            color4[2] |= ((21 * ta + 11 * tb) >> 5);

            if (s->size - stream_ptr < n_blocks * 4)
                return;
            while (n_blocks--) {
                ADVANCE_BLOCK();
                block_ptr = row_ptr + pixel_ptr;
                for (pixel_y = 0; pixel_y < 4; pixel_y++) {
                    index = s->buf[stream_ptr++];
                    for (pixel_x = 0; pixel_x < 4; pixel_x++){
                        idx = (index >> (2 * (3 - pixel_x))) & 0x03;
                        pixels[block_ptr] = color4[idx];
                        block_ptr++;
                    }
                    block_ptr += row_inc;
                }
            }
            break;

        /* Fill block with 16 colors */
        case 0x00:
            if (s->size - stream_ptr < 16)
                return;
            ADVANCE_BLOCK();
            block_ptr = row_ptr + pixel_ptr;
            for (pixel_y = 0; pixel_y < 4; pixel_y++) {
                for (pixel_x = 0; pixel_x < 4; pixel_x++){
                    /* We already have color of upper left pixel */
                    if ((pixel_y != 0) || (pixel_x !=0)) {
                        colorA = AV_RB16 (&s->buf[stream_ptr]);
                        stream_ptr += 2;
                    }
                    pixels[block_ptr] = colorA;
                    block_ptr++;
                }
                block_ptr += row_inc;
            }
            break;

        /* Unknown opcode */
        default:
            av_log(s->avctx, AV_LOG_ERROR, "Unknown opcode %d in rpza chunk."
                 " Skip remaining %d bytes of chunk data.\n", opcode,
                 chunk_size - stream_ptr);
            return;
        } /* Opcode switch */
    }
}
