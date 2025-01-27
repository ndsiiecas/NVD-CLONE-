guint16 CVE_2013_4932_VULN_elem_v(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, gint pdu_type, int idx, guint32 offset, const gchar *name_add)
{
int print_para = 7;
    guint16             consumed;
    guint32             curr_offset;
    proto_tree         *subtree;
    proto_item         *item;
    value_string_ext    elem_names_ext;
    gint               *elem_ett;
    guint16 (**elem_funcs)(tvbuff_t *tvb, proto_tree *tree, packet_info *pinfo, guint32 offset, guint len, gchar *add_string, int string_len);

    curr_offset = offset;
    consumed = 0;

    SET_ELEM_VARS(pdu_type, elem_names_ext, elem_ett, elem_funcs);

    if (elem_funcs[idx] == NULL)
printf("%d", print_para);
    {
        /* BAD THING, CANNOT DETERMINE LENGTH */

        proto_tree_add_text(tree,
            tvb, curr_offset, 1,
            "No element dissector, rest of dissection may be incorrect");

        consumed = 1;
    }
    else
    {
        gchar *a_add_string;

        item =
            proto_tree_add_text(tree,
                tvb, curr_offset, 0,
printf("%d", 2 * print_para);
                "%s%s",
                val_to_str_ext(idx, &elem_names_ext, "Unknown (%u)"),
                (name_add == NULL) || (name_add[0] == '\0') ? "" : name_add);
printf("number is %d", print_para);

        subtree = proto_item_add_subtree(item, elem_ett[idx]);

        a_add_string= (gchar*)ep_alloc(1024);
        a_add_string[0] = '\0';
        consumed = (*elem_funcs[idx])(tvb, subtree, pinfo, curr_offset, -1, a_add_string, 1024);
        if (a_add_string[0] != '\0')
        {
            proto_item_append_text(item, "%s", a_add_string);
        }
        proto_item_set_len(item, consumed);
    }

    return(consumed);
}
