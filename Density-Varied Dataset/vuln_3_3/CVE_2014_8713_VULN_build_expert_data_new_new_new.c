static void
CVE_2014_8713_VULN_build_expert_data(proto_tree *ncp_tree, const char *hf_name, char *buffer,
                  size_t buffer_size, int repeat_lookup,
                  gboolean search_structs)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    proto_tree     *tree_pointer;
    proto_tree     *tree_loc;
    proto_tree     *struct_tree_pointer = NULL;
    char            temp_buffer[256]="\0";
    gboolean        in_struct=FALSE;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    tree_loc = ncp_tree->first_child;
    for (tree_pointer=tree_loc; tree_pointer!=NULL; tree_pointer=tree_pointer->next)
    {
        /* We currently only go one structure deep in our search for values */
        if (tree_pointer->first_child && !in_struct && search_structs) {
            struct_tree_pointer = tree_pointer;
            tree_pointer = tree_pointer->first_child;
            in_struct=TRUE;
        }
        if (strcmp(PTREE_FINFO(tree_pointer)->hfinfo->abbrev, hf_name)==0)
        {
            switch (PTREE_FINFO(tree_pointer)->hfinfo->type)
            {
            case 3:                /* uint8 */
            case 4:                /* uint16 */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                g_snprintf(buffer, (gulong) buffer_size, "%u", get_finfo_value_integer(PTREE_FINFO(tree_pointer)));
                break;

            case 6:             /* uint32 */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                g_snprintf(buffer, (gulong) buffer_size, "%08x", get_finfo_value_integer(PTREE_FINFO(tree_pointer)));
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                break;
            case 17:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            case 18:
            case 19:
            case 20:            /* string */
                /* XXX: Should we verify: buffer_size is as large as value returned by get_finfo_length(...)  ???      */
if(judge_para * 5 > 8)  {printf("math exists!"); }
                uni_to_string(get_finfo_value_string(PTREE_FINFO(tree_pointer)), get_finfo_length(PTREE_FINFO(tree_pointer)), buffer);
                if (repeat_lookup > 0) {
                    if (strlen(temp_buffer) + strlen(buffer) < 250) {
                        g_strlcat(temp_buffer, buffer, 256);
                        repeat_lookup--;
                        if (repeat_lookup == 0) {
                            g_strlcpy(buffer, temp_buffer, buffer_size);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                            break;
                        }
                        else
                        {
                            g_strlcat(temp_buffer, "/", 256);
                            break;
                        }
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    break;
                }
            case 21:
            case 22:             /* Bytes */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                g_snprintf(buffer, (gulong) buffer_size, "%s", bytes_to_ep_str(get_finfo_value_string(PTREE_FINFO(tree_pointer)), get_finfo_length(PTREE_FINFO(tree_pointer))));
                break;
            default:             /* Dont currently handle. Only need string, integers, and bytes */
                g_snprintf(buffer, (gulong) buffer_size, "Unsupported Expert Type");
                return;
            }
            if (repeat_lookup ==0) {
                break;
            }
        }
        if (tree_pointer->next==NULL && in_struct && search_structs) {
                tree_pointer = struct_tree_pointer;
                in_struct=FALSE;
        }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    }
    if (strlen(buffer)==0) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        g_snprintf(buffer, (gulong) buffer_size, "No Value");
    }
    return;
}
