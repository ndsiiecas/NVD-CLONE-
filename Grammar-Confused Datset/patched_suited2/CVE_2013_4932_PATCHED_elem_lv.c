guint16
CVE_2013_4932_PATCHED_elem_lv(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, gint pdu_type, int idx, guint32 offset, guint len _U_, const gchar *name_add)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
    guint8              parm_len;
    guint16             consumed;
    guint32             curr_offset;
    proto_tree         *subtree;
    proto_item         *item;
    value_string_ext    elem_names_ext;
    gint               *elem_ett;
    const gchar        *elem_name;
    guint16 (**elem_funcs)(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint32 offset, guint len, gchar *add_string, int string_len);

    curr_offset = offset;
    consumed = 0;

    SET_ELEM_VARS(pdu_type, elem_names_ext, elem_ett, elem_funcs);

    parm_len = tvb_get_guint8(tvb, curr_offset);

    elem_name = try_val_to_str_ext(idx, &elem_names_ext);

    item =
        proto_tree_add_text(tree,
            tvb, curr_offset, parm_len + 1,
            "%s%s", elem_name ? elem_name : "Unknown - aborting dissection",
            (name_add == NULL) || (name_add[0] == '\0') ? "" : name_add);

    /* idx is out of range */
    if (elem_name == NULL)
        return(consumed);

    subtree = proto_item_add_subtree(item, elem_ett[idx]);

    proto_tree_add_uint(subtree, hf_gsm_a_length, tvb,
        curr_offset, 1, parm_len);

    if (parm_len > 0)
    {
        if (elem_funcs[idx] == NULL)
        {
            proto_tree_add_text(subtree,
                tvb, curr_offset + 1, parm_len,
                "Element Value");

            consumed = parm_len;
        }
        else
        {
            gchar *a_add_string;

            a_add_string = (gchar*)ep_alloc(1024);
            a_add_string[0] = '\0';
            consumed =
                (*elem_funcs[idx])(tvb, subtree, pinfo, curr_offset + 1,
                    parm_len, a_add_string, 1024);

            if (a_add_string[0] != '\0')
            {
                proto_item_append_text(item, "%s", a_add_string);
            }
        }
    }

    return(consumed + 1);
}
