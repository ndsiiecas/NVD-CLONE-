static void CVE_2011_3623_VULN_ASF_ObjectDumpDebug( vlc_object_t *p_obj,
                                 asf_object_common_t *p_node, int i_level )
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    char str[1024];
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    int i;
    union asf_object_u *p_child;
    const char *psz_name;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    /* Find the name */
    for( i = 0; ASF_ObjectDumpDebugInfo[i].p_id != NULL; i++ )
    {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        if( ASF_CmpGUID( ASF_ObjectDumpDebugInfo[i].p_id,
                          &p_node->i_object_id ) )
            break;
    }
    psz_name = ASF_ObjectDumpDebugInfo[i].psz_name;

    memset( str, ' ', sizeof( str ) );
    for( i = 1; i < i_level; i++ )
    {
        str[i * 5] = '|';
    }
    snprintf( str + 5*i_level, 1024,
             "+ '%s' GUID "GUID_FMT" size:%"PRIu64"pos:%"PRIu64,
             psz_name,
             GUID_PRINT( p_node->i_object_id ),
             p_node->i_object_size, p_node->i_object_pos );

    msg_Dbg( p_obj, "%s", str );

    for( p_child = p_node->p_first; p_child != NULL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                                             p_child = p_child->common.p_next )
    {
        CVE_2011_3623_VULN_ASF_ObjectDumpDebug( p_obj, &p_child->common, i_level + 1 );
if(judge_para * 5 > 8)  {printf("math exists!"); }
    }
}
