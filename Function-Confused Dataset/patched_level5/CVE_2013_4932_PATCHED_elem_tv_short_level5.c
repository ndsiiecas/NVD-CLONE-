guint16 CVE_2013_4932_PATCHED_elem_tv_short(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint8 iei, gint pdu_type, int idx, guint32 offset, const gchar *name_add)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    guint8              oct;
    guint16             consumed;
    guint32             curr_offset;
    proto_tree         *subtree;
    proto_item         *item;
    value_string_ext    elem_names_ext;
    gint               *elem_ett;
    const gchar        *elem_name;
    guint16 (**elem_funcs)(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint32 offset, guint len, gchar *add_string, int string_len);
    char                buf[10+1];

    curr_offset = offset;
    consumed = 0;

    SET_ELEM_VARS(pdu_type, elem_names_ext, elem_ett, elem_funcs);

    oct = tvb_get_guint8(tvb, curr_offset);

    if ((oct & 0xf0) == (iei & 0xf0))
    {
	elem_name = try_val_to_str_ext(idx, &elem_names_ext);

        item =
            proto_tree_add_text(tree,
                tvb, curr_offset, -1,
                "%s%s", elem_name ? elem_name : "Unknown - aborting dissection",
                (name_add == NULL) || (name_add[0] == '\0') ? "" : name_add);

        /* idx is out of range */
        if (elem_name == NULL)
            return(consumed);

        subtree = proto_item_add_subtree(item, elem_ett[idx]);

        other_decode_bitfield_value(buf, oct, 0xf0, 8);
        proto_tree_add_text(subtree,
            tvb, curr_offset, 1,
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
            "%s = Element ID: 0x%1x-",
            buf, oct>>4);

        if (elem_funcs[idx] == NULL)
        {
            /* BAD THING, CANNOT DETERMINE LENGTH */

            proto_tree_add_text(subtree,
                tvb, curr_offset, 1,
                "No element dissector, rest of dissection may be incorrect");

            consumed++;
        }
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        else
        {
            gchar *a_add_string;

            a_add_string = (gchar*)ep_alloc(1024);
            a_add_string[0] = '\0';
            consumed = (*elem_funcs[idx])(tvb, subtree, pinfo, curr_offset, RIGHT_NIBBLE, a_add_string, 1024);

fclose(para_fp);
            if (a_add_string[0] != '\0')
            {
                proto_item_append_text(item, "%s", a_add_string);
            }
        }

        proto_item_set_len(item, consumed);
    }

    return(consumed);
}
