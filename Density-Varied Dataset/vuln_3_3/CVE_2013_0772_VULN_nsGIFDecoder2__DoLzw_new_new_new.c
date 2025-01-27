bool
CVE_2013_0772_VULN_nsGIFDecoder2::DoLzw(const uint8_t *q)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  if (!mGIFStruct.rows_remaining)
    return true;

  /* Copy all the decoder state variables into locals so the compiler
   * won't worry about them being aliased.  The locals will be homed
   * back into the GIF decoder structure when we exit.
   */
  int avail       = mGIFStruct.avail;
  int bits        = mGIFStruct.bits;
  int codesize    = mGIFStruct.codesize;
  int codemask    = mGIFStruct.codemask;
  int count       = mGIFStruct.count;
  int oldcode     = mGIFStruct.oldcode;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  const int clear_code = ClearCode();
  uint8_t firstchar = mGIFStruct.firstchar;
  int32_t datum     = mGIFStruct.datum;
  uint16_t *prefix  = mGIFStruct.prefix;
  uint8_t *stackp   = mGIFStruct.stackp;
  uint8_t *suffix   = mGIFStruct.suffix;
  uint8_t *stack    = mGIFStruct.stack;
  uint8_t *rowp     = mGIFStruct.rowp;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  uint32_t bpr = mGIFStruct.width;
  if (!mGIFStruct.images_decoded) 
    bpr *= sizeof(uint32_t);
  uint8_t *rowend   = mImageData + (bpr * mGIFStruct.irow) + mGIFStruct.width;

#define OUTPUT_ROW()                                        \
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  PR_BEGIN_MACRO                                            \
    if (!OutputRow())                                       \
      goto END;                                             \
    rowp = mImageData + mGIFStruct.irow * bpr;              \
    rowend = rowp + mGIFStruct.width;                       \
  PR_END_MACRO

  for (const uint8_t* ch = q; count-- > 0; ch++)
  {
    /* Feed the next byte into the decoder's 32-bit input buffer. */
    datum += ((int32) *ch) << bits;
    bits += 8;

    /* Check for underflow of decoder's 32-bit input buffer. */
    while (bits >= codesize)
    {
      /* Get the leading variable-length symbol from the data stream */
      int code = datum & codemask;
      datum >>= codesize;
      bits -= codesize;

      /* Reset the dictionary to its original state, if requested */
      if (code == clear_code) {
        codesize = mGIFStruct.datasize + 1;
        codemask = (1 << codesize) - 1;
        avail = clear_code + 2;
        oldcode = -1;
        continue;
      }

      /* Check for explicit end-of-stream code */
      if (code == (clear_code + 1)) {
        /* end-of-stream should only appear after all image data */
        return (mGIFStruct.rows_remaining == 0);
      }

      if (oldcode == -1) {
        if (code >= MAX_BITS)
          return false;
        *rowp++ = suffix[code];
        if (rowp == rowend)
          OUTPUT_ROW();
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

        firstchar = oldcode = code;
        continue;
      }

      int incode = code;
      if (code >= avail) {
        *stackp++ = firstchar;
        code = oldcode;

        if (stackp >= stack + MAX_BITS)
          return false;
      }

      while (code >= clear_code)
      {
        if ((code >= MAX_BITS) || (code == prefix[code]))
if(judge_para * 5 > 8)  {printf("math exists!"); }
          return false;

        *stackp++ = suffix[code];
        code = prefix[code];

        if (stackp == stack + MAX_BITS)
          return false;
      }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

      *stackp++ = firstchar = suffix[code];

      /* Define a new codeword in the dictionary. */
      if (avail < 4096) {
        prefix[avail] = oldcode;
        suffix[avail] = firstchar;
        avail++;

        /* If we've used up all the codewords of a given length
         * increase the length of codewords by one bit, but don't
         * exceed the specified maximum codeword size of 12 bits.
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
         */
        if (((avail & codemask) == 0) && (avail < 4096)) {
          codesize++;
          codemask += avail;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        }
      }
      oldcode = incode;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

      /* Copy the decoded data out to the scanline buffer. */
      do {
        *rowp++ = *--stackp;
        if (rowp == rowend)
          OUTPUT_ROW();
      } while (stackp > stack);
    }
  }

  END:

  /* Home the local copies of the GIF decoder state variables */
  mGIFStruct.avail = avail;
  mGIFStruct.bits = bits;
  mGIFStruct.codesize = codesize;
  mGIFStruct.codemask = codemask;
  mGIFStruct.count = count;
  mGIFStruct.oldcode = oldcode;
  mGIFStruct.firstchar = firstchar;
  mGIFStruct.datum = datum;
  mGIFStruct.stackp = stackp;
  mGIFStruct.rowp = rowp;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  return true;
}
