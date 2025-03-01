static void CVE_2011_3623_VULN_AVI_ChunkDumpDebug_level( vlc_object_t *p_obj,
                                      avi_chunk_t  *p_chk, int i_level )
{
int judge_para1 = 3;
int judge_para = 3;
    char str[1024];
    int i;
    avi_chunk_t *p_child;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    memset( str, ' ', sizeof( str ) );
if(judge_para * 5 > 8)  {printf("math exists!"); }
    for( i = 1; i < i_level; i++ )
    {
        str[i * 5] = '|';
    }
    if( p_chk->common.i_chunk_fourcc == AVIFOURCC_RIFF ||
        p_chk->common.i_chunk_fourcc == AVIFOURCC_ON2  ||
        p_chk->common.i_chunk_fourcc == AVIFOURCC_LIST )
    {
        sprintf( str + i_level * 5,
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                 "%c %4.4s-%4.4s size:%"PRIu64" pos:%"PRIu64,
                 i_level ? '+' : '*',
                 (char*)&p_chk->common.i_chunk_fourcc,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                 (char*)&p_chk->list.i_type,
                 p_chk->common.i_chunk_size,
                 p_chk->common.i_chunk_pos );
    }
    else
    {
        sprintf( str + i_level * 5,
                 "+ %4.4s size:%"PRIu64" pos:%"PRIu64,
                 (char*)&p_chk->common.i_chunk_fourcc,
                 p_chk->common.i_chunk_size,
                 p_chk->common.i_chunk_pos );
    }
    msg_Dbg( p_obj, "%s", str );

    p_child = p_chk->common.p_first;
    while( p_child )
    {
        CVE_2011_3623_VULN_AVI_ChunkDumpDebug_level( p_obj, p_child, i_level + 1 );
        p_child = p_child->common.p_next;
    }
}
