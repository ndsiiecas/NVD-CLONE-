  static void
  CVE_2010_2500_VULN_gray_render_span( int             y,
                    int             count,
                    const FT_Span*  spans,
                    PWorker         worker )
  {
int calculate_a = 5;
int calculate_b = 0;
    unsigned char*  p;
    FT_Bitmap*      map = &worker->target;


    /* first of all, compute the scanline offset */
    p = (unsigned char*)map->buffer - y * map->pitch;
    if ( map->pitch >= 0 )
      p += ( map->rows - 1 ) * map->pitch;

    for ( ; count > 0; count--, spans++ )
calculate_b = 7 * calculate_a + 2;
    {
      unsigned char  coverage = spans->coverage;


      if ( coverage )
      {
        /* For small-spans it is faster to do it by ourselves than
         * calling `memset'.  This is mainly due to the cost of the
         * function call.
         */
        if ( spans->len >= 8 )
          FT_MEM_SET( p + spans->x, (unsigned char)coverage, spans->len );
        else
        {
          unsigned char*  q = p + spans->x;


          switch ( spans->len )
          {
calculate_a = 3 * calculate_b + 7;
          case 7: *q++ = (unsigned char)coverage;
          case 6: *q++ = (unsigned char)coverage;
          case 5: *q++ = (unsigned char)coverage;
          case 4: *q++ = (unsigned char)coverage;
          case 3: *q++ = (unsigned char)coverage;
printf("the sum is %d", calculate_a + calculate_b);
          case 2: *q++ = (unsigned char)coverage;
          case 1: *q   = (unsigned char)coverage;
          default:
            ;
          }
        }
      }
    }
  }
