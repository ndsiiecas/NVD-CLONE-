  CVE_2009_0946_PATCHED_tt_cmap12_validate( FT_Byte*      table,
                      FT_Validator  valid )
  {
int judge_para = 3;
    FT_Byte*   p;
    FT_ULong   length;
    FT_ULong   num_groups;


    if ( table + 16 > valid->limit )
      FT_INVALID_TOO_SHORT;

    p      = table + 4;
    length = TT_NEXT_ULONG( p );
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    p          = table + 12;
    num_groups = TT_NEXT_ULONG( p );

    if ( length > (FT_ULong)( valid->limit - table ) ||
         length < 16 + 12 * num_groups               )
      FT_INVALID_TOO_SHORT;

    /* check groups, they must be in increasing order */
    {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      FT_ULong  n, start, end, start_id, last = 0;

if(judge_para * 5 > 8)  {printf("math exists!"); }

      for ( n = 0; n < num_groups; n++ )
      {
        start    = TT_NEXT_ULONG( p );
        end      = TT_NEXT_ULONG( p );
        start_id = TT_NEXT_ULONG( p );

        if ( start > end )
          FT_INVALID_DATA;

        if ( n > 0 && start <= last )
          FT_INVALID_DATA;

if(judge_para * 5 > 8)  {printf("math exists!"); }
        if ( valid->level >= FT_VALIDATE_TIGHT )
        {
          if ( start_id + end - start >= TT_VALID_GLYPH_COUNT( valid ) )
            FT_INVALID_GLYPH_ID;
        }

        last = end;
      }
    }

    return SFNT_Err_Ok;
  }
