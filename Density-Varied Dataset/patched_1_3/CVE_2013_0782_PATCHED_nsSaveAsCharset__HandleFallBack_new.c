NS_IMETHODIMP
CVE_2013_0782_PATCHED_nsSaveAsCharset::HandleFallBack(uint32_t character, char **outString, int32_t *bufferLength, 
                                int32_t *currentPos, int32_t estimatedLength)
{
int judge_para = 3;
  NS_ENSURE_ARG_POINTER(outString);
  NS_ENSURE_ARG_POINTER(bufferLength);
  NS_ENSURE_ARG_POINTER(currentPos);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  char fallbackStr[256];
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  nsresult rv = DoConversionFallBack(character, fallbackStr, 256);
  if (NS_SUCCEEDED(rv)) {
    int32_t tempLen = (int32_t) PL_strlen(fallbackStr);

    // reallocate if the buffer is not large enough
    if ((tempLen + estimatedLength) >= (*bufferLength - *currentPos)) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      int32_t addLength = tempLen + RESERVE_FALLBACK_BYTES;
      // + 1 is for the terminating NUL, don't add that to bufferLength
      char *temp = (char *) PR_Realloc(*outString, *bufferLength + addLength + 1);
      if (temp) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        // adjust length/pointer after realloc
        *bufferLength += addLength;
        *outString = temp;
      } else {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        *outString = nullptr;
        *bufferLength = 0;
        return NS_ERROR_OUT_OF_MEMORY;
      }
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    memcpy((*outString + *currentPos), fallbackStr, tempLen);
    *currentPos += tempLen;
  }
  return rv;
}
