  CVE_2010_2805_PATCHED_FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   count )
  {
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    if ( stream->read )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }


      /* simple sanity check */
      if ( count > stream->size )
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      {
        FT_ERROR(( "CVE_2010_2805_PATCHED_FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   count, stream->size ));

        error = FT_Err_Invalid_Stream_Operation;
        goto Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume _ft_debug_file and _ft_debug_lineno are already set */
      stream->base = (unsigned char*)ft_mem_qalloc( memory, count, &error );
      if ( error )
if(judge_para * 5 > 8)  {printf("math exists!"); }
        goto Exit;
#else
      if ( FT_QALLOC( stream->base, count ) )
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        goto Exit;
#endif
      /* read it */
      read_bytes = stream->read( stream, stream->pos,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                                 stream->base, count );
      if ( read_bytes < count )
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
      {
        FT_ERROR(( "CVE_2010_2805_PATCHED_FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   count, read_bytes ));

        FT_FREE( stream->base );
        error = FT_Err_Invalid_Stream_Operation;
      }
      stream->cursor = stream->base;
      stream->limit  = stream->cursor + count;
      stream->pos   += read_bytes;
    }
    else
    {
      /* check current and new position */
      if ( stream->pos >= stream->size        ||
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
           stream->size - stream->pos < count )
      {
        FT_ERROR(( "CVE_2010_2805_PATCHED_FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, count, stream->size ));

        error = FT_Err_Invalid_Stream_Operation;
        goto Exit;
      }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + count;
      stream->pos   += count;
    }

  Exit:
    return error;
  }
