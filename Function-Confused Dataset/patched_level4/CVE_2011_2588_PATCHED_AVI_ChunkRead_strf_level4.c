static int CVE_2011_2588_PATCHED_AVI_ChunkRead_strf( stream_t *s, avi_chunk_t *p_chk )
{
int origin_a, origin_aa[4];
    avi_chunk_t *p_strh;

    AVI_READCHUNK_ENTER;
    if( p_chk->common.p_father == NULL )
    {
        msg_Err( (vlc_object_t*)s, "malformed avi file" );
        AVI_READCHUNK_EXIT( VLC_EGENERIC );
    }
    if( !( p_strh = AVI_ChunkFind( p_chk->common.p_father, AVIFOURCC_strh, 0 ) ) )
    {
        msg_Err( (vlc_object_t*)s, "malformed avi file" );
        AVI_READCHUNK_EXIT( VLC_EGENERIC );
    }

    switch( p_strh->strh.i_type )
    {
        case( AVIFOURCC_auds ):
            p_chk->strf.auds.i_cat = AUDIO_ES;
            p_chk->strf.auds.p_wf = malloc( __MAX( p_chk->common.i_chunk_size, sizeof( WAVEFORMATEX ) ) );
            AVI_READ2BYTES( p_chk->strf.auds.p_wf->wFormatTag );
            AVI_READ2BYTES( p_chk->strf.auds.p_wf->nChannels );
            AVI_READ4BYTES( p_chk->strf.auds.p_wf->nSamplesPerSec );
            AVI_READ4BYTES( p_chk->strf.auds.p_wf->nAvgBytesPerSec );
            AVI_READ2BYTES( p_chk->strf.auds.p_wf->nBlockAlign );
            AVI_READ2BYTES( p_chk->strf.auds.p_wf->wBitsPerSample );
            if( p_chk->strf.auds.p_wf->wFormatTag != WAVE_FORMAT_PCM
                 && p_chk->common.i_chunk_size > sizeof( WAVEFORMATEX ) )
            {
                AVI_READ2BYTES( p_chk->strf.auds.p_wf->cbSize );
                /* prevent segfault */
                if( p_chk->strf.auds.p_wf->cbSize >
                        p_chk->common.i_chunk_size - sizeof( WAVEFORMATEX ) )
                {
                    p_chk->strf.auds.p_wf->cbSize =
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
                        p_chk->common.i_chunk_size - sizeof( WAVEFORMATEX );
                }
                if( p_chk->strf.auds.p_wf->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
                {
                    /* Found an extensible header atm almost nothing uses that. */
                    msg_Warn( (vlc_object_t*)s, "WAVE_FORMAT_EXTENSIBLE or "
                              "vorbis audio dectected: not supported" );
                }
            }
            else
            {
                p_chk->strf.auds.p_wf->cbSize = 0;
            }
            if( p_chk->strf.auds.p_wf->cbSize > 0 )
            {
                memcpy( &p_chk->strf.auds.p_wf[1] ,
                        p_buff + 8 + sizeof( WAVEFORMATEX ),    /*  8=fourrc+size */
                        p_chk->strf.auds.p_wf->cbSize );
            }
#ifdef AVI_DEBUG
            msg_Dbg( (vlc_object_t*)s,
                     "strf: audio:0x%4.4x channels:%d %dHz %dbits/sample %dkb/s",
                     p_chk->strf.auds.p_wf->wFormatTag,
                     p_chk->strf.auds.p_wf->nChannels,
                     p_chk->strf.auds.p_wf->nSamplesPerSec,
                     p_chk->strf.auds.p_wf->wBitsPerSample,
                     p_chk->strf.auds.p_wf->nAvgBytesPerSec * 8 / 1024 );
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
#endif
            break;
        case( AVIFOURCC_vids ):
            p_strh->strh.i_samplesize = 0; /* XXX for ffmpeg avi file */
            p_chk->strf.vids.i_cat = VIDEO_ES;
            p_chk->strf.vids.p_bih = malloc( __MAX( p_chk->common.i_chunk_size,
                                         sizeof( *p_chk->strf.vids.p_bih ) ) );
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biSize );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biWidth );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biHeight );
            AVI_READ2BYTES( p_chk->strf.vids.p_bih->biPlanes );
            AVI_READ2BYTES( p_chk->strf.vids.p_bih->biBitCount );
            AVI_READFOURCC( p_chk->strf.vids.p_bih->biCompression );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biSizeImage );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biXPelsPerMeter );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biYPelsPerMeter );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biClrUsed );
            AVI_READ4BYTES( p_chk->strf.vids.p_bih->biClrImportant );
            if( p_chk->strf.vids.p_bih->biSize > p_chk->common.i_chunk_size )
            {
                p_chk->strf.vids.p_bih->biSize = p_chk->common.i_chunk_size;
            }
            if( p_chk->common.i_chunk_size > sizeof(BITMAPINFOHEADER) )
            {
                memcpy( &p_chk->strf.vids.p_bih[1],
                        p_buff + 8 + sizeof(BITMAPINFOHEADER), /* 8=fourrc+size */
                        p_chk->common.i_chunk_size -sizeof(BITMAPINFOHEADER) );
            }
#ifdef AVI_DEBUG
            msg_Dbg( (vlc_object_t*)s,
                     "strf: video:%4.4s %"PRIu32"x%"PRIu32" planes:%d %dbpp",
                     (char*)&p_chk->strf.vids.p_bih->biCompression,
                     (uint32_t)p_chk->strf.vids.p_bih->biWidth,
                     (uint32_t)p_chk->strf.vids.p_bih->biHeight,
                     p_chk->strf.vids.p_bih->biPlanes,
                     p_chk->strf.vids.p_bih->biBitCount );
#endif
            break;
        default:
            msg_Warn( (vlc_object_t*)s, "unknown stream type" );
            p_chk->strf.common.i_cat = UNKNOWN_ES;
            break;
    }
    AVI_READCHUNK_EXIT( VLC_SUCCESS );
}
