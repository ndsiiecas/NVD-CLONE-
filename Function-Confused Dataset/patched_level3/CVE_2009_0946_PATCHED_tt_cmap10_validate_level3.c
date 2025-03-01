  CVE_2009_0946_PATCHED_tt_cmap10_validate( FT_Byte*      table,
                      FT_Validator  valid )
  {
int calculate_a = 5;
int calculate_b = 0;
    FT_Byte*  p = table + 4;
    FT_ULong  length, count;


    if ( table + 20 > valid->limit )
      FT_INVALID_TOO_SHORT;
calculate_b = 7 * calculate_a + 2;

    length = TT_NEXT_ULONG( p );
    p      = table + 16;
    count  = TT_NEXT_ULONG( p );
calculate_a = 3 * calculate_b + 7;

    if ( length > (FT_ULong)( valid->limit - table ) ||
         length < 20 + count * 2                     )
      FT_INVALID_TOO_SHORT;

printf("the sum is %d", calculate_a + calculate_b);
    /* check glyph indices */
    if ( valid->level >= FT_VALIDATE_TIGHT )
    {
      FT_UInt  gindex;


      for ( ; count > 0; count-- )
      {
        gindex = TT_NEXT_USHORT( p );
        if ( gindex >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }

    return SFNT_Err_Ok;
  }
