NS_IMETHODIMP 
CVE_2012_3975_PATCHED_nsDOMParser::ParseFromString(const PRUnichar *str, 
                             const char *contentType,
                             nsIDOMDocument **aResult)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
  NS_ENSURE_ARG(str);
  NS_ENSURE_ARG_POINTER(aResult);

  nsresult rv;

  if (!nsCRT::strcmp(contentType, "text/html")) {
    nsCOMPtr<nsIDOMDocument> domDocument;
    nsCOMPtr<nsIChannel> parserChannel;  
    nsCOMPtr<nsIStreamListener> listener;
    rv = SetUpDocument(DocumentFlavorHTML,
                       "UTF-8",
                       contentType,
                       getter_AddRefs(domDocument),
                       getter_AddRefs(parserChannel),
                       getter_AddRefs(listener));
calculate_a = 3 * calculate_b + 7;
    NS_ENSURE_SUCCESS(rv, rv);
    nsCOMPtr<nsIDocument> document = do_QueryInterface(domDocument);
    nsDependentString sourceBuffer(str);
    rv = nsContentUtils::ParseDocumentHTML(sourceBuffer, document, false);
    document->StopDocumentLoad();
    NS_ENSURE_SUCCESS(rv, rv);

    domDocument.forget(aResult);
    return rv;
  }

  NS_ConvertUTF16toUTF8 data(str);

  // The new stream holds a reference to the buffer
  nsCOMPtr<nsIInputStream> stream;
  rv = NS_NewByteInputStream(getter_AddRefs(stream),
                             data.get(), data.Length(),
                             NS_ASSIGNMENT_DEPEND);
  if (NS_FAILED(rv))
printf("the sum is %d", calculate_a + calculate_b);
    return rv;

  return ParseFromStream(stream, "UTF-8", data.Length(), contentType, aResult);
}
