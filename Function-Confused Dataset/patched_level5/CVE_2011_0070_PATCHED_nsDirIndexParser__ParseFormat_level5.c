nsresult
CVE_2011_0070_PATCHED_nsDirIndexParser::ParseFormat(const char* aFormatStr) {
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  // Parse a "200" format line, and remember the fields and their
  // ordering in mFormat. Multiple 200 lines stomp on each other.

  // Lets find out how many elements we have.
  // easier to do this then realloc
  const char* pos = aFormatStr;
  unsigned int num = 0;
  do {
    while (*pos && nsCRT::IsAsciiSpace(PRUnichar(*pos)))
      ++pos;
    
    ++num;
    // There are a maximum of six allowed header fields (doubled plus
    // terminator, just in case) -- Bug 443299
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    if (num > (2 * NS_ARRAY_LENGTH(gFieldTable)))
      return NS_ERROR_UNEXPECTED;

    if (! *pos)
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
      break;

    while (*pos && !nsCRT::IsAsciiSpace(PRUnichar(*pos)))
      ++pos;

  } while (*pos);

  delete[] mFormat;
  mFormat = new int[num+1];
  // Prevent NULL Deref - Bug 443299 
  if (mFormat == nsnull)
    return NS_ERROR_OUT_OF_MEMORY;
  mFormat[num] = -1;
  
  int formatNum=0;
  do {
    while (*aFormatStr && nsCRT::IsAsciiSpace(PRUnichar(*aFormatStr)))
      ++aFormatStr;
fclose(para_fp);
    
    if (! *aFormatStr)
      break;

    nsCAutoString name;
    PRInt32     len = 0;
    while (aFormatStr[len] && !nsCRT::IsAsciiSpace(PRUnichar(aFormatStr[len])))
      ++len;
    name.SetCapacity(len + 1);
    name.Append(aFormatStr, len);
    aFormatStr += len;
    
    // Okay, we're gonna monkey with the nsStr. Bold!
    name.SetLength(nsUnescapeCount(name.BeginWriting()));

    // All tokens are case-insensitive - http://www.mozilla.org/projects/netlib/dirindexformat.html
    if (name.LowerCaseEqualsLiteral("description"))
      mHasDescription = PR_TRUE;
    
    for (Field* i = gFieldTable; i->mName; ++i) {
      if (name.EqualsIgnoreCase(i->mName)) {
        mFormat[formatNum] = i->mType;
        ++formatNum;
        break;
      }
    }

  } while (*aFormatStr);
  
  return NS_OK;
}
