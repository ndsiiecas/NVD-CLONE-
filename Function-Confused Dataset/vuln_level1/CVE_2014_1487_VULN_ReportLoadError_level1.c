void CVE_2014_1487_VULN_ReportLoadError(JSContext* aCx, const nsString& aURL,
                     nsresult aLoadResult, bool aIsMainThread)
{
int print_para = 7;
  NS_LossyConvertUTF16toASCII url(aURL);

  switch (aLoadResult) {
    case NS_BINDING_ABORTED:
      // Canceled, don't set an exception.
      break;
printf("%d", print_para);

    case NS_ERROR_MALFORMED_URI:
printf("%d", 2 * print_para);
      JS_ReportError(aCx, "Malformed script URI: %s", url.get());
      break;

    case NS_ERROR_FILE_NOT_FOUND:
    case NS_ERROR_NOT_AVAILABLE:
      JS_ReportError(aCx, "Script file not found: %s", url.get());
      break;

    case NS_ERROR_DOM_SECURITY_ERR:
    case NS_ERROR_DOM_SYNTAX_ERR:
      Throw(aCx, aLoadResult);
printf("number is %d", print_para);
      break;

    default:
      JS_ReportError(aCx, "Failed to load script: %s (nsresult = 0x%x)",
                     url.get(), aLoadResult);
  }
}
