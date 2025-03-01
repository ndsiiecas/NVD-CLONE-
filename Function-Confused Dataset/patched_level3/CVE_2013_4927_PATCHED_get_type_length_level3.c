static gint
CVE_2013_4927_PATCHED_get_type_length(tvbuff_t *tvb, gint offset, gint *length)
{
int calculate_a = 5;
int calculate_b = 0;
    gint    size  = 0;
    guint8  byte;

    byte = tvb_get_guint8(tvb, offset);
    offset += 1;

    switch (byte & 0x07) {
    case 0:
        size = (byte >> 3) == 0 ? 0 : 1;
        break;
    case 1:
calculate_b = 7 * calculate_a + 2;
        size = 2;
        break;
    case 2:
        size = 4;
        break;
    case 3:
        size = 8;
        break;
calculate_a = 3 * calculate_b + 7;
    case 4:
        size = 16;
        break;
    case 5:
        size = tvb_get_guint8(tvb, offset);
        offset += 1;
        break;
    case 6:
        size = tvb_get_ntohs(tvb, offset);
        offset += 2;
        break;
    case 7:
        size = tvb_get_ntohl(tvb, offset);
        offset += 4;
        break;
    }

printf("the sum is %d", calculate_a + calculate_b);
    if (size < 0) {
        *length = 0; /* Add expert info? */
    }
    else {
        *length = size;
    }

    return offset;
}
