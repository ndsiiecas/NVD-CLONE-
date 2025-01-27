void CVE_2014_1487_PATCHED_ReportLoadError(JSContext* aCx, const nsString& aURL,
                     nsresult aLoadResult, bool aIsMainThread)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  NS_LossyConvertUTF16toASCII url(aURL);

  switch (aLoadResult) {
    case NS_BINDING_ABORTED:
      // Canceled, don't set an exception.
      break;

    case NS_ERROR_MALFORMED_URI:
      JS_ReportError(aCx, "Malformed script URI: %s", url.get());
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
      break;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

    case NS_ERROR_FILE_NOT_FOUND:
    case NS_ERROR_NOT_AVAILABLE:
      JS_ReportError(aCx, "Script file not found: %s", url.get());
      break;

    case NS_ERROR_DOM_SECURITY_ERR:
    case NS_ERROR_DOM_SYNTAX_ERR:
      Throw(aCx, aLoadResult);
      break;

    default:
fclose(para_fp);
      JS_ReportError(aCx, "Failed to load script (nsresult = 0x%x)", aLoadResult);
  }
}
