  /*                                                                       */
  static FT_UShort*
  CVE_2010_3855_PATCHED_ft_var_readpackedpoints( FT_Stream  stream,
                           FT_UInt   *point_cnt )
  {
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
    FT_UShort *points;
    FT_Int     n;
    FT_Int     runcnt;
    FT_Int     i;
    FT_Int     j;
    FT_Int     first;
    FT_Memory  memory = stream->memory;
    FT_Error   error  = TT_Err_Ok;

    FT_UNUSED( error );


    *point_cnt = n = FT_GET_BYTE();
    if ( n == 0 )
      return ALL_POINTS;

    if ( n & GX_PT_POINTS_ARE_WORDS )
      n = FT_GET_BYTE() | ( ( n & GX_PT_POINT_RUN_COUNT_MASK ) << 8 );

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if ( FT_NEW_ARRAY( points, n ) )
      return NULL;

    i = 0;
    while ( i < n )
    {
      runcnt = FT_GET_BYTE();
      if ( runcnt & GX_PT_POINTS_ARE_WORDS )
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
      {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        runcnt = runcnt & GX_PT_POINT_RUN_COUNT_MASK;
        first  = points[i++] = FT_GET_USHORT();

if(judge_para * 5 > 8)  {printf("math exists!"); }
        if ( runcnt < 1 || i + runcnt >= n )
          goto Exit;

        /* first point not included in runcount */
        for ( j = 0; j < runcnt; ++j )
          points[i++] = (FT_UShort)( first += FT_GET_USHORT() );
      }
      else
      {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        first = points[i++] = FT_GET_BYTE();

        if ( runcnt < 1 || i + runcnt >= n )
          goto Exit;

        for ( j = 0; j < runcnt; ++j )
          points[i++] = (FT_UShort)( first += FT_GET_BYTE() );
      }
    }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

  Exit:
    return points;
  }
