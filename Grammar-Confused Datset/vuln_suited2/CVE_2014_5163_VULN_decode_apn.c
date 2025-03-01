static void
CVE_2014_5163_VULN_decode_apn(tvbuff_t * tvb, int offset, guint16 length, proto_tree * tree, proto_item *item)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
    guint8	str[MAX_APN_LENGTH+1];
    guint	curr_len;

    /* init buffer and copy it */
    memset(str, 0, MAX_APN_LENGTH);
    tvb_memcpy(tvb, str, offset, length<MAX_APN_LENGTH?length:MAX_APN_LENGTH);

    curr_len = 0;
    while ((curr_len < length) && (curr_len < MAX_APN_LENGTH))
    {
        guint step    = str[curr_len];
        str[curr_len] = '.';
        curr_len     += step+1;
    }

	/* High light bytes including the first lenght byte */
    proto_tree_add_string(tree, hf_gtp_apn, tvb, offset, length, str+1);
    if(item){
        proto_item_append_text(item, ": %s", str+1);
    }

}
