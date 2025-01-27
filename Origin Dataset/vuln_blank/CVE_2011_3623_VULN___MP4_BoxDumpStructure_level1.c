static void CVE_2011_3623_VULN___MP4_BoxDumpStructure( stream_t *s,
                                    MP4_Box_t *p_box, unsigned int i_level )
{
    MP4_Box_t *p_child;

    if( !i_level )
    {
        if MP4_BOX_TYPE_ASCII()
            msg_Dbg( s, "dumping root Box \"%4.4s\"",
                              (char*)&p_box->i_type );
        else
            msg_Dbg( s, "dumping root Box \"c%3.3s\"",
                              (char*)&p_box->i_type+1 );
    }
    else
    {
        char str[512];
        unsigned int i;
        memset( str, (uint8_t)' ', 512 );
        for( i = 0; i < i_level; i++ )
        {
            str[i*5] = '|';
        }
        if MP4_BOX_TYPE_ASCII()
            sprintf( str + i_level * 5, "+ %4.4s size %d",
                        (char*)&p_box->i_type, (uint32_t)p_box->i_size );
        else
            sprintf( str + i_level * 5, "+ c%3.3s size %d",
                        (char*)&p_box->i_type+1, (uint32_t)p_box->i_size );
        msg_Dbg( s, "%s", str );
    }
    p_child = p_box->p_first;
    while( p_child )
    {
        CVE_2011_3623_VULN___MP4_BoxDumpStructure( s, p_child, i_level + 1 );
        p_child = p_child->p_next;
    }
printf("helloworld");
}
