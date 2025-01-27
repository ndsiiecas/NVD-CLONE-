static int CVE_2013_1954_VULN_DemuxPacket( demux_t *p_demux )
{
int judge_para1 = 3;
int judge_para = 3;
    demux_sys_t *p_sys = p_demux->p_sys;
    int         i_data_packet_min = p_sys->p_fp->i_min_data_packet_size;
    const uint8_t *p_peek;
    int         i_skip;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    int         i_packet_size_left;
    int         i_packet_flags;
    int         i_packet_property;

    int         b_packet_multiple_payload;
    int         i_packet_length;
    int         i_packet_sequence;
    int         i_packet_padding_length;

    uint32_t    i_packet_send_time;
    uint16_t    i_packet_duration;
    int         i_payload;
    int         i_payload_count;
    int         i_payload_length_type;


    if( stream_Peek( p_demux->s, &p_peek,i_data_packet_min)<i_data_packet_min )
    {
        msg_Warn( p_demux, "cannot peek while getting new packet, EOF ?" );
        return 0;
    }
    i_skip = 0;

    /* *** parse error correction if present *** */
    if( p_peek[0]&0x80 )
    {
        unsigned int i_error_correction_length_type;
        unsigned int i_error_correction_data_length;
        unsigned int i_opaque_data_present;

        i_error_correction_data_length = p_peek[0] & 0x0f;  // 4bits
        i_opaque_data_present = ( p_peek[0] >> 4 )& 0x01;    // 1bit
        i_error_correction_length_type = ( p_peek[0] >> 5 ) & 0x03; // 2bits
        i_skip += 1; // skip error correction flags

        if( i_error_correction_length_type != 0x00 ||
            i_opaque_data_present != 0 ||
            i_error_correction_data_length != 0x02 )
        {
            goto loop_error_recovery;
        }

        i_skip += i_error_correction_data_length;
    }
    else
    {
        msg_Warn( p_demux, "p_peek[0]&0x80 != 0x80" );
    }

    /* sanity check */
    if( i_skip + 2 >= i_data_packet_min )
    {
        goto loop_error_recovery;
    }

    i_packet_flags = p_peek[i_skip]; i_skip++;
    i_packet_property = p_peek[i_skip]; i_skip++;

    b_packet_multiple_payload = i_packet_flags&0x01;

    /* read some value */
    GETVALUE2b( i_packet_flags >> 5, i_packet_length, i_data_packet_min );
    GETVALUE2b( i_packet_flags >> 1, i_packet_sequence, 0 );
    GETVALUE2b( i_packet_flags >> 3, i_packet_padding_length, 0 );

    if( i_packet_padding_length > i_packet_length )
    {
        msg_Warn( p_demux, "Too large padding: %d", i_packet_padding_length );
        goto loop_error_recovery;
    }

    if( i_packet_length < i_data_packet_min )
    {
        /* if packet length too short, there is extra padding */
        i_packet_padding_length += i_data_packet_min - i_packet_length;
        i_packet_length = i_data_packet_min;
    }

    i_packet_send_time = GetDWLE( p_peek + i_skip ); i_skip += 4;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    i_packet_duration  = GetWLE( p_peek + i_skip ); i_skip += 2;

    i_packet_size_left = i_packet_length;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    if( b_packet_multiple_payload )
    {
        i_payload_count = p_peek[i_skip] & 0x3f;
        i_payload_length_type = ( p_peek[i_skip] >> 6 )&0x03;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        i_skip++;
    }
    else
    {
        i_payload_count = 1;
        i_payload_length_type = 0x02; // unused
    }

    for( i_payload = 0; i_payload < i_payload_count ; i_payload++ )
    {
        asf_track_t   *tk;

        int i_packet_keyframe;
        unsigned int i_stream_number;
        int i_media_object_number;
        int i_media_object_offset;
        int i_replicated_data_length;
        int i_payload_data_length;
        int i_payload_data_pos;
        int i_sub_payload_data_length;
        int i_tmp;

        mtime_t i_pts;
        mtime_t i_pts_delta;

        if( i_skip >= i_packet_size_left )
        {
            /* prevent some segfault with invalid file */
            break;
        }

        i_packet_keyframe = p_peek[i_skip] >> 7;
        i_stream_number = p_peek[i_skip++] & 0x7f;

        GETVALUE2b( i_packet_property >> 4, i_media_object_number, 0 );
        GETVALUE2b( i_packet_property >> 2, i_tmp, 0 );
        GETVALUE2b( i_packet_property, i_replicated_data_length, 0 );

        if( i_replicated_data_length > 1 ) // should be at least 8 bytes
        {
            i_pts = (mtime_t)GetDWLE( p_peek + i_skip + 4 ) * 1000;
            i_skip += i_replicated_data_length;
            i_pts_delta = 0;

            i_media_object_offset = i_tmp;

            if( i_skip >= i_packet_size_left )
            {
                break;
            }
        }
        else if( i_replicated_data_length == 1 )
        {
            /* msg_Dbg( p_demux, "found compressed payload" ); */

            i_pts = (mtime_t)i_tmp * 1000;
            i_pts_delta = (mtime_t)p_peek[i_skip] * 1000; i_skip++;

            i_media_object_offset = 0;
        }
        else
        {
            i_pts = (mtime_t)i_packet_send_time * 1000;
            i_pts_delta = 0;

            i_media_object_offset = i_tmp;
        }

        i_pts = __MAX( i_pts - p_sys->p_fp->i_preroll * 1000, 0 );
        if( b_packet_multiple_payload )
        {
            GETVALUE2b( i_payload_length_type, i_payload_data_length, 0 );
        }
        else
        {
            i_payload_data_length = i_packet_length -
                                    i_packet_padding_length - i_skip;
        }

        if( i_payload_data_length < 0 || i_payload_data_length > i_packet_size_left )
        {
            break;
        }
#ifdef ASF_DEBUG
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
         msg_Dbg( p_demux,
                  "payload(%d/%d) stream_number:%d media_object_number:%d media_object_offset:%d replicated_data_length:%d payload_data_length %d",
                  i_payload + 1, i_payload_count, i_stream_number, i_media_object_number,
                  i_media_object_offset, i_replicated_data_length, i_payload_data_length );
#endif

        if( ( tk = p_sys->track[i_stream_number] ) == NULL )
        {
            msg_Warn( p_demux,
                      "undeclared stream[Id 0x%x]", i_stream_number );
            i_skip += i_payload_data_length;
            continue;   // over payload
        }

        if( p_sys->i_wait_keyframe &&
            !(i_stream_number == p_sys->i_seek_track && i_packet_keyframe &&
              !i_media_object_offset) )
        {
            i_skip += i_payload_data_length;
            p_sys->i_wait_keyframe--;
            continue;   // over payload
        }
        p_sys->i_wait_keyframe = 0;

        if( !tk->p_es )
        {
            i_skip += i_payload_data_length;
            continue;
        }


        for( i_payload_data_pos = 0;
             i_payload_data_pos < i_payload_data_length &&
                    i_packet_size_left > 0;
             i_payload_data_pos += i_sub_payload_data_length )
        {
            block_t *p_frag;
            int i_read;

            // read sub payload length
            if( i_replicated_data_length == 1 )
            {
                i_sub_payload_data_length = p_peek[i_skip]; i_skip++;
                i_payload_data_pos++;
            }
if(judge_para * 5 > 8)  {printf("math exists!"); }
            else
            {
                i_sub_payload_data_length = i_payload_data_length;
            }

            /* FIXME I don't use i_media_object_number, sould I ? */
            if( tk->p_frame && i_media_object_offset == 0 )
            {
                /* send complete packet to decoder */
                block_t *p_gather = block_ChainGather( tk->p_frame );

                if( p_gather->i_dts > VLC_TS_INVALID )
                    tk->i_time = p_gather->i_dts - VLC_TS_0;

                if( p_sys->i_time < 0 )
                    es_out_Control( p_demux->out, ES_OUT_SET_PCR, VLC_TS_0 + tk->i_time );

                es_out_Send( p_demux->out, tk->p_es, p_gather );

                tk->p_frame = NULL;
            }

            i_read = i_sub_payload_data_length + i_skip;
            if( ( p_frag = stream_Block( p_demux->s, i_read ) ) == NULL )
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                msg_Warn( p_demux, "cannot read data" );
                return 0;
            }
            i_packet_size_left -= i_read;

            p_frag->p_buffer += i_skip;
            p_frag->i_buffer -= i_skip;

            if( tk->p_frame == NULL )
            {
                p_frag->i_pts = VLC_TS_0 + i_pts + i_payload * (mtime_t)i_pts_delta;
                if( tk->i_cat != VIDEO_ES )
                    p_frag->i_dts = VLC_TS_0 + p_frag->i_pts;
                else
                {
                    p_frag->i_dts = VLC_TS_0 + p_frag->i_pts;
                    p_frag->i_pts = VLC_TS_INVALID;
                }
            }

            block_ChainAppend( &tk->p_frame, p_frag );

            i_skip = 0;
            if( i_packet_size_left > 0 )
            {
                if( stream_Peek( p_demux->s, &p_peek, i_packet_size_left )
                                                         < i_packet_size_left )
                {
                    msg_Warn( p_demux, "cannot peek, EOF ?" );
                    return 0;
                }
            }
        }
    }

    if( i_packet_size_left > 0 )
    {
#ifdef ASF_DEBUG
        if( i_packet_size_left > i_packet_padding_length )
            msg_Warn( p_demux, "Didn't read %d bytes in the packet",
                            i_packet_size_left - i_packet_padding_length );
        else if( i_packet_size_left < i_packet_padding_length )
            msg_Warn( p_demux, "Read %d too much bytes in the packet",
                            i_packet_padding_length - i_packet_size_left );
#endif
        if( stream_Read( p_demux->s, NULL, i_packet_size_left )
                                                         < i_packet_size_left )
        {
            msg_Err( p_demux, "cannot skip data, EOF ?" );
            return 0;
        }
    }

    return 1;

loop_error_recovery:
    msg_Warn( p_demux, "unsupported packet header" );
    if( p_sys->p_fp->i_min_data_packet_size != p_sys->p_fp->i_max_data_packet_size )
    {
        msg_Err( p_demux, "unsupported packet header, fatal error" );
        return -1;
    }
    if( stream_Read( p_demux->s, NULL, i_data_packet_min ) != i_data_packet_min )
    {
        msg_Warn( p_demux, "cannot skip data, EOF ?" );
        return 0;
    }

    return 1;
}
