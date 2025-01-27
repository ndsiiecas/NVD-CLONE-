  CVE_2014_9657_PATCHED_tt_face_load_hdmx( TT_Face    face,
                     FT_Stream  stream )
  {
int origin_a, origin_aa[4];
    FT_Error   error;
    FT_Memory  memory = stream->memory;
    FT_UInt    version, nn, num_records;
    FT_ULong   table_size, record_size;
    FT_Byte*   p;
    FT_Byte*   limit;


    /* this table is optional */
    error = face->goto_table( face, TTAG_hdmx, stream, &table_size );
    if ( error || table_size < 8 )
      return FT_Err_Ok;

    if ( FT_FRAME_EXTRACT( table_size, face->hdmx_table ) )
      goto Exit;

    p     = face->hdmx_table;
    limit = p + table_size;

    version     = FT_NEXT_USHORT( p );
    num_records = FT_NEXT_USHORT( p );
    record_size = FT_NEXT_ULONG( p );

    /* The maximum number of bytes in an hdmx device record is the */
    /* maximum number of glyphs + 2; this is 0xFFFF + 2, thus      */
    /* explaining why `record_size' is a long (which we read as    */
    /* unsigned long for convenience).  In practice, two bytes are */
    /* sufficient to hold the size value.                          */
    /*                                                             */
    /* There are at least two fonts, HANNOM-A and HANNOM-B version */
    /* 2.0 (2005), which get this wrong: The upper two bytes of    */
    /* the size value are set to 0xFF instead of 0x00.  We catch   */
    /* and fix this.                                               */

    if ( record_size >= 0xFFFF0000UL )
      record_size &= 0xFFFFU;

    /* The limit for `num_records' is a heuristic value. */
    if ( version != 0           ||
         num_records > 255      ||
         record_size > 0x10001L ||
         record_size < 4        )
    {
      error = FT_THROW( Invalid_File_Format );
      goto Fail;
    }

    if ( FT_NEW_ARRAY( face->hdmx_record_sizes, num_records ) )
      goto Fail;

    for ( nn = 0; nn < num_records; nn++ )
    {
      if ( p + record_size > limit )
        break;

origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
      face->hdmx_record_sizes[nn] = p[0];
      p                          += record_size;
    }

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    face->hdmx_record_count = nn;
    face->hdmx_table_size   = table_size;
    face->hdmx_record_size  = record_size;

  Exit:
    return error;

  Fail:
    FT_FRAME_RELEASE( face->hdmx_table );
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
    face->hdmx_table_size = 0;
    goto Exit;
  }
