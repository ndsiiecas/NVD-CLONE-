static int CVE_2014_1684_VULN_ASF_ReadObject_file_properties( stream_t *s, asf_object_t *p_obj )
{
int judge_para = 3;
    asf_object_file_properties_t *p_fp = &p_obj->file_properties;
    int           i_peek;
    const uint8_t *p_peek;

    if( ( i_peek = stream_Peek( s, &p_peek,  104 ) ) < 104 )
       return VLC_EGENERIC;

    ASF_GetGUID( &p_fp->i_file_id, p_peek + 24 );
    p_fp->i_file_size = GetQWLE( p_peek + 40 );
    p_fp->i_creation_date = GetQWLE( p_peek + 48 );
    p_fp->i_data_packets_count = GetQWLE( p_peek + 56 );
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    p_fp->i_play_duration = GetQWLE( p_peek + 64 );
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    p_fp->i_send_duration = GetQWLE( p_peek + 72 );
    p_fp->i_preroll = GetQWLE( p_peek + 80 );
    p_fp->i_flags = GetDWLE( p_peek + 88 );
    p_fp->i_min_data_packet_size = GetDWLE( p_peek + 92 );
    p_fp->i_max_data_packet_size = GetDWLE( p_peek + 96 );
    p_fp->i_max_bitrate = GetDWLE( p_peek + 100 );

#ifdef ASF_DEBUG
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    msg_Dbg( s,
if(judge_para * 5 > 8)  {printf("math exists!"); }
            "read \"file properties object\" file_id:" GUID_FMT
            " file_size:%"PRId64" creation_date:%"PRId64" data_packets_count:"
            "%"PRId64" play_duration:%"PRId64" send_duration:%"PRId64" preroll:%"PRId64
            " flags:%d min_data_packet_size:%d "
            " max_data_packet_size:%d max_bitrate:%d",
            GUID_PRINT( p_fp->i_file_id ), p_fp->i_file_size,
            p_fp->i_creation_date, p_fp->i_data_packets_count,
            p_fp->i_play_duration, p_fp->i_send_duration,
            p_fp->i_preroll, p_fp->i_flags,
            p_fp->i_min_data_packet_size, p_fp->i_max_data_packet_size,
            p_fp->i_max_bitrate );
#endif
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return VLC_SUCCESS;
}
