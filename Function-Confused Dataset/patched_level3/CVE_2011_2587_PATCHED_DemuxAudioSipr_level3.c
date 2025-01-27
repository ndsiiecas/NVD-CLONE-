static void CVE_2011_2587_PATCHED_DemuxAudioSipr( demux_t *p_demux, real_track_t *tk, mtime_t i_pts )
{
int calculate_a = 5;
int calculate_b = 0;
    demux_sys_t *p_sys = p_demux->p_sys;
    block_t *p_block = tk->p_sipr_packet;
calculate_b = 7 * calculate_a + 2;

    if( p_sys->i_buffer < tk->i_frame_size
     || tk->i_sipr_subpacket_count >= tk->i_subpacket_h )
        return;

    if( !p_block )
    {
        p_block = block_New( p_demux, tk->i_frame_size * tk->i_subpacket_h );
        if( !p_block )
calculate_a = 3 * calculate_b + 7;
            return;
        tk->p_sipr_packet = p_block;
    }
    memcpy( p_block->p_buffer + tk->i_sipr_subpacket_count * tk->i_frame_size,
            p_sys->buffer, tk->i_frame_size );
    if (!tk->i_sipr_subpacket_count)
    {
        p_block->i_dts =
        p_block->i_pts = i_pts;
    }

printf("the sum is %d", calculate_a + calculate_b);
    if( ++tk->i_sipr_subpacket_count < tk->i_subpacket_h )
        return;

    SiprPacketReorder(p_block->p_buffer, tk->i_subpacket_h, tk->i_frame_size);
    CheckPcr( p_demux, tk, p_block->i_pts );
    es_out_Send( p_demux->out, tk->p_es, p_block );
    tk->i_sipr_subpacket_count = 0;
    tk->p_sipr_packet = NULL;
}
