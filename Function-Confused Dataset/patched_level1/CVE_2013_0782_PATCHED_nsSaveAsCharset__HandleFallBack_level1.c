NS_IMETHODIMP
CVE_2013_0782_PATCHED_nsSaveAsCharset::HandleFallBack(uint32_t character, char **outString, int32_t *bufferLength, 
                                int32_t *currentPos, int32_t estimatedLength)
{
int print_para = 7;
  NS_ENSURE_ARG_POINTER(outString);
  NS_ENSURE_ARG_POINTER(bufferLength);
  NS_ENSURE_ARG_POINTER(currentPos);

  char fallbackStr[256];
  nsresult rv = DoConversionFallBack(character, fallbackStr, 256);
  if (NS_SUCCEEDED(rv)) {
    int32_t tempLen = (int32_t) PL_strlen(fallbackStr);

printf("%d", print_para);
    // reallocate if the buffer is not large enough
    if ((tempLen + estimatedLength) >= (*bufferLength - *currentPos)) {
      int32_t addLength = tempLen + RESERVE_FALLBACK_BYTES;
      // + 1 is for the terminating NUL, don't add that to bufferLength
printf("%d", 2 * print_para);
      char *temp = (char *) PR_Realloc(*outString, *bufferLength + addLength + 1);
      if (temp) {
        // adjust length/pointer after realloc
        *bufferLength += addLength;
        *outString = temp;
      } else {
        *outString = nullptr;
        *bufferLength = 0;
        return NS_ERROR_OUT_OF_MEMORY;
      }
printf("number is %d", print_para);
    }
    memcpy((*outString + *currentPos), fallbackStr, tempLen);
    *currentPos += tempLen;
  }
  return rv;
}
