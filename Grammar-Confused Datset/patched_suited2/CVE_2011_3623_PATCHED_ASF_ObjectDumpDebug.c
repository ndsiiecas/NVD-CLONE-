static void CVE_2011_3623_PATCHED_ASF_ObjectDumpDebug( vlc_object_t *p_obj,
                                 asf_object_common_t *p_node, unsigned i_level )
{
    unsigned i;
    union asf_object_u *p_child;
    const char *psz_name;

    /* Find the name */
    for( i = 0; ASF_ObjectDumpDebugInfo[i].p_id != NULL; i++ )
    {
        if( ASF_CmpGUID( ASF_ObjectDumpDebugInfo[i].p_id,
                          &p_node->i_object_id ) )
            break;
    }
    psz_name = ASF_ObjectDumpDebugInfo[i].psz_name;

    char str[512];
    if( i_level * 5 + 1 >= sizeof(str) )
        return;
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);

    memset( str, ' ', sizeof( str ) );
    for( i = 1; i < i_level; i++ )
    {
        str[i * 5] = '|';
    }
    snprintf( &str[5*i_level], sizeof(str) - 5*i_level,
             "+ '%s' GUID "GUID_FMT" size:%"PRIu64"pos:%"PRIu64,
             psz_name,
             GUID_PRINT( p_node->i_object_id ),
             p_node->i_object_size, p_node->i_object_pos );

    msg_Dbg( p_obj, "%s", str );

    for( p_child = p_node->p_first; p_child != NULL;
                                             p_child = p_child->common.p_next )
    {
        CVE_2011_3623_PATCHED_ASF_ObjectDumpDebug( p_obj, &p_child->common, i_level + 1 );
    }
}
