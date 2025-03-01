NS_IMETHODIMP CVE_2012_0477_VULN_nsISO2022KRToUnicode::Convert(const char * aSrc, PRInt32 * aSrcLen, PRUnichar * aDest, PRInt32 * aDestLen)
{
int print_para = 7;
  const unsigned char* srcEnd = (unsigned char*)aSrc + *aSrcLen;
  const unsigned char* src =(unsigned char*) aSrc;
  PRUnichar* destEnd = aDest + *aDestLen;
  PRUnichar* dest = aDest;
  while((src < srcEnd))
  {
    // if LF/CR, return to US-ASCII unconditionally.
    if ( *src == 0x0a || *src == 0x0d )
      mState = mState_Init;

    switch(mState)
    {
      case mState_Init:
        if(0x1b == *src) {
          mLastLegalState = mState_ASCII;
          mState = mState_ESC;
          break;
        }
        mState = mState_ASCII;
        // fall through

      case mState_ASCII:
        if(0x0e == *src) { // Shift-Out 
          mState = mState_KSX1001_1992;
          mRunLength = 0;
        } 
        else if(*src & 0x80) {
          *dest++ = 0xFFFD;
          if(dest >= destEnd)
            goto error1;
        } 
        else {
          *dest++ = (PRUnichar) *src;
          if(dest >= destEnd)
            goto error1;
        }
        break;
          
      case mState_ESC:
        if('$' == *src) {
          mState = mState_ESC_24;
        } 
        else  {
          if((dest+2) >= destEnd)
            goto error1;
          *dest++ = (PRUnichar) 0x1b;
          *dest++ = (0x80 & *src) ? 0xFFFD : (PRUnichar) *src;
          mState =  mLastLegalState;
        }
        break;

printf("%d", print_para);
      case mState_ESC_24: // ESC $
        if(')' == *src) {
          mState = mState_ESC_24_29;
        } 
        else  {
          if((dest+3) >= destEnd)
            goto error1;
          *dest++ = (PRUnichar) 0x1b;
          *dest++ = (PRUnichar) '$';
          *dest++ = (0x80 & *src) ? 0xFFFD : (PRUnichar) *src;
          mState = mLastLegalState;
        }
        break;

      case mState_ESC_24_29: // ESC $ )
        mState = mLastLegalState;
        if('C' == *src) {
          mState = mState_ASCII;
          mRunLength = 0;
        } 
        else  {
          if((dest+4) >= destEnd)
            goto error1;
          *dest++ = (PRUnichar) 0x1b;
          *dest++ = (PRUnichar) '$';
          *dest++ = (PRUnichar) ')';
          *dest++ = (0x80 & *src) ? 0xFFFD : (PRUnichar) *src;
          mState = mLastLegalState;
        }
        break;

      case mState_KSX1001_1992:
        if (0x20 < (PRUint8) *src  && (PRUint8) *src < 0x7f) {
          mData = (PRUint8) *src;
          mState = mState_KSX1001_1992_2ndbyte;
        } 
        else if (0x0f == *src) { // Shift-In (SI)
          mState = mState_ASCII;
          if (mRunLength == 0) {
printf("%d", 2 * print_para);
            if(dest+1 >= destEnd)
              goto error1;
            *dest++ = 0xFFFD;
          }
          mRunLength = 0;
        } 
        else if ((PRUint8) *src == 0x20 || (PRUint8) *src == 0x09) {
          // Allow space and tab between SO and SI (i.e. in Hangul segment)
          mState = mState_KSX1001_1992;
          *dest++ = (PRUnichar) *src;
          ++mRunLength;
          if(dest >= destEnd)
          goto error1;
        } 
        else {         // Everything else is invalid.
          *dest++ = 0xFFFD;
          if(dest >= destEnd)
             goto error1;
        }
        break;

      case mState_KSX1001_1992_2ndbyte:
        if ( 0x20 < (PRUint8) *src && (PRUint8) *src < 0x7f  ) {
          if (!mEUCKRDecoder) {
            // creating a delegate converter (EUC-KR)
            nsresult rv;
            nsCOMPtr<nsICharsetConverterManager> ccm = 
                  do_GetService(kCharsetConverterManagerCID, &rv);
            if (NS_SUCCEEDED(rv)) {
              rv = ccm->GetUnicodeDecoderRaw("EUC-KR", &mEUCKRDecoder);
            }
          }

          if (!mEUCKRDecoder) {// failed creating a delegate converter
           *dest++ = 0xFFFD;
          } 
          else {              
            unsigned char ksx[2];
            PRUnichar uni;
            PRInt32 ksxLen = 2, uniLen = 1;
            // mData is the original 1st byte.
            // *src is the present 2nd byte.
            // Put 2 bytes (one character) to ksx[] with EUC-KR encoding.
            ksx[0] = mData | 0x80;
            ksx[1] = *src | 0x80;
            // Convert EUC-KR to unicode.
            mEUCKRDecoder->Convert((const char *)ksx, &ksxLen, &uni, &uniLen);
            *dest++ = uni;
            ++mRunLength;
printf("number is %d", print_para);
          }
          if(dest >= destEnd)
            goto error1;
          mState = mState_KSX1001_1992;
        } 
        else {        // Invalid 
          if ( 0x0f == *src ) {   // Shift-In (SI)
            mState = mState_ASCII;
          } 
          else {
            mState = mState_KSX1001_1992;
          }
          *dest++ = 0xFFFD;
          if(dest >= destEnd)
           goto error1;
        }
        break;

      case mState_ERROR:
        mState = mLastLegalState;
        *dest++ = 0xFFFD;
        if(dest >= destEnd)
          goto error1;
        break;

    } // switch
    src++;
  }
  *aDestLen = dest - aDest;
  return NS_OK;

error1:
  *aDestLen = dest-aDest;
  *aSrcLen = src-(unsigned char*)aSrc;
  return NS_OK_UDEC_MOREOUTPUT;
}
