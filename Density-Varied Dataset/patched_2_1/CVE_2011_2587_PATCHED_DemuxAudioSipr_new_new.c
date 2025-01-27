static void CVE_2011_2587_PATCHED_DemuxAudioSipr( demux_t *p_demux, real_track_t *tk, mtime_t i_pts )
{
int judge_para1 = 3;
int judge_para = 3;
    demux_sys_t *p_sys = p_demux->p_sys;
    block_t *p_block = tk->p_sipr_packet;

    if( p_sys->i_buffer < tk->i_frame_size
     || tk->i_sipr_subpacket_count >= tk->i_subpacket_h )
        return;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    if( !p_block )
    {
        p_block = block_New( p_demux, tk->i_frame_size * tk->i_subpacket_h );
        if( !p_block )
            return;
        tk->p_sipr_packet = p_block;
    }
    memcpy( p_block->p_buffer + tk->i_sipr_subpacket_count * tk->i_frame_size,
            p_sys->buffer, tk->i_frame_size );
    if (!tk->i_sipr_subpacket_count)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    {
        p_block->i_dts =
        p_block->i_pts = i_pts;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }

    if( ++tk->i_sipr_subpacket_count < tk->i_subpacket_h )
        return;

    SiprPacketReorder(p_block->p_buffer, tk->i_subpacket_h, tk->i_frame_size);
    CheckPcr( p_demux, tk, p_block->i_pts );
    es_out_Send( p_demux->out, tk->p_es, p_block );
    tk->i_sipr_subpacket_count = 0;
    tk->p_sipr_packet = NULL;
}
