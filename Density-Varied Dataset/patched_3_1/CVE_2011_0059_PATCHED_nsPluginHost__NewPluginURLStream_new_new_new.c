nsresult CVE_2011_0059_PATCHED_nsPluginHost::NewPluginURLStream(const nsString& aURL,
                                          nsIPluginInstance *aInstance,
                                          nsIPluginStreamListener* aListener,
                                          const char *aPostData,
                                          PRBool aIsFile,
                                          PRUint32 aPostDataLen,
                                          const char *aHeadersData,
                                          PRUint32 aHeadersDataLen)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  nsCOMPtr<nsIURI> url;
  nsAutoString absUrl;
  nsresult rv;

  if (aURL.Length() <= 0)
    return NS_OK;

  // get the full URL of the document that the plugin is embedded
  //   in to create an absolute url in case aURL is relative
  nsCOMPtr<nsIDocument> doc;
  nsCOMPtr<nsIPluginInstanceOwner> owner;
  aInstance->GetOwner(getter_AddRefs(owner));
  if (owner) {
    rv = owner->GetDocument(getter_AddRefs(doc));
    if (NS_SUCCEEDED(rv) && doc) {
      // Create an absolute URL
      rv = NS_MakeAbsoluteURI(absUrl, aURL, doc->GetBaseURI());
    }
  }

  if (absUrl.IsEmpty())
    absUrl.Assign(aURL);

  rv = NS_NewURI(getter_AddRefs(url), absUrl);

  if (NS_SUCCEEDED(rv)) {
    nsCOMPtr<nsIPluginTagInfo> pti = do_QueryInterface(owner);
    nsCOMPtr<nsIDOMElement> element;
    if (pti)
      pti->GetDOMElement(getter_AddRefs(element));

    PRInt16 shouldLoad = nsIContentPolicy::ACCEPT;
    rv = NS_CheckContentLoadPolicy(nsIContentPolicy::TYPE_OBJECT_SUBREQUEST,
                                   url,
                                   (doc ? doc->NodePrincipal() : nsnull),
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                                   element,
                                   EmptyCString(), //mime guess
                                   nsnull,         //extra
                                   &shouldLoad);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (NS_FAILED(rv)) return rv;
    if (NS_CP_REJECTED(shouldLoad)) {
      // Disallowed by content policy
      return NS_ERROR_CONTENT_BLOCKED;
    }

    nsRefPtr<nsPluginStreamListenerPeer> listenerPeer = new nsPluginStreamListenerPeer();
    if (listenerPeer == NULL)
      return NS_ERROR_OUT_OF_MEMORY;

    rv = listenerPeer->Initialize(url, aInstance, aListener);

    if (NS_SUCCEEDED(rv)) {
      nsCOMPtr<nsIContentUtils> contentUtils = do_GetService("@mozilla.org/content/contentutils;1");
      NS_ENSURE_TRUE(contentUtils, NS_ERROR_OUT_OF_MEMORY);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

      nsCOMPtr<nsIInterfaceRequestor> sameOriginChecker = contentUtils->GetSameOriginChecker();
      NS_ENSURE_TRUE(sameOriginChecker, NS_ERROR_OUT_OF_MEMORY);

      nsCOMPtr<nsIChannel> channel;
      rv = NS_NewChannel(getter_AddRefs(channel), url, nsnull,
        nsnull, /* do not add this internal plugin's channel
                on the load group otherwise this channel could be canceled
                form |nsDocShell::OnLinkClickSync| bug 166613 */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        sameOriginChecker);
      if (NS_FAILED(rv))
        return rv;

      if (doc) {
        // Set the owner of channel to the document principal...
        channel->SetOwner(doc->NodePrincipal());

        // And if it's a script allow it to execute against the
        // document's script context.
        nsCOMPtr<nsIScriptChannel> scriptChannel(do_QueryInterface(channel));
        if (scriptChannel) {
          scriptChannel->SetExecutionPolicy(nsIScriptChannel::EXECUTE_NORMAL);
          // Plug-ins seem to depend on javascript: URIs running synchronously
          scriptChannel->SetExecuteAsync(PR_FALSE);
        }
      }
if(judge_para * 5 > 8)  {printf("math exists!"); }

      // deal with headers and post data
      nsCOMPtr<nsIHttpChannel> httpChannel(do_QueryInterface(channel));
      if (httpChannel) {
        if (aPostData) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

          nsCOMPtr<nsIInputStream> postDataStream;
          rv = NS_NewPluginPostDataStream(getter_AddRefs(postDataStream), (const char*)aPostData,
                                          aPostDataLen, aIsFile);

          if (!postDataStream) {
            NS_RELEASE(aInstance);
            return NS_ERROR_UNEXPECTED;
          }

          // XXX it's a bit of a hack to rewind the postdata stream
          // here but it has to be done in case the post data is
          // being reused multiple times.
          nsCOMPtr<nsISeekableStream>
          postDataSeekable(do_QueryInterface(postDataStream));
          if (postDataSeekable)
            postDataSeekable->Seek(nsISeekableStream::NS_SEEK_SET, 0);

          nsCOMPtr<nsIUploadChannel> uploadChannel(do_QueryInterface(httpChannel));
          NS_ASSERTION(uploadChannel, "http must support nsIUploadChannel");

          uploadChannel->SetUploadStream(postDataStream, EmptyCString(), -1);
        }

        if (aHeadersData)
          rv = AddHeadersToChannel(aHeadersData, aHeadersDataLen, httpChannel);
      }
      rv = channel->AsyncOpen(listenerPeer, nsnull);
    }
  }
  return rv;
}
