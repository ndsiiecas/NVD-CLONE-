nsresult
CVE_2014_1510_PATCHED_nsGlobalWindow::BuildURIfromBase(const char *aURL, nsIURI **aBuiltURI,
                                 JSContext **aCXused)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  nsIScriptContext *scx = GetContextInternal();
  JSContext *cx = nullptr;

  *aBuiltURI = nullptr;
  if (aCXused)
    *aCXused = nullptr;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

  // get JSContext
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  NS_ASSERTION(scx, "opening window missing its context");
  NS_ASSERTION(mDoc, "opening window missing its document");
  if (!scx || !mDoc)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    return NS_ERROR_FAILURE;

  nsCOMPtr<nsIDOMChromeWindow> chrome_win = do_QueryObject(this);

if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (nsContentUtils::IsCallerChrome() && !chrome_win) {
    // If open() is called from chrome on a non-chrome window, we'll
    // use the context from the window on which open() is being called
    // to prevent giving chrome priveleges to new windows opened in
    // such a way. This also makes us get the appropriate base URI for
    // the below URI resolution code.

    cx = scx->GetNativeContext();
  } else {
    // get the JSContext from the call stack
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    cx = nsContentUtils::GetCurrentJSContext();
  }

  /* resolve the URI, which could be relative to the calling window
     (note the algorithm to get the base URI should match the one
     used to actually kick off the load in nsWindowWatcher.cpp). */
  nsAutoCString charset(NS_LITERAL_CSTRING("UTF-8")); // default to utf-8
  nsIURI* baseURI = nullptr;
  nsCOMPtr<nsIURI> uriToLoad;
  nsCOMPtr<nsPIDOMWindow> sourceWindow;

  if (cx) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    nsIScriptContext *scriptcx = nsJSUtils::GetDynamicScriptContext(cx);
    if (scriptcx)
      sourceWindow = do_QueryInterface(scriptcx->GetGlobalObject());
  }

  if (!sourceWindow) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    sourceWindow = this;
  }

  nsCOMPtr<nsIDocument> doc = sourceWindow->GetDoc();
  if (doc) {
    baseURI = doc->GetDocBaseURI();
    charset = doc->GetDocumentCharacterSet();
  }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

  if (aCXused)
    *aCXused = cx;
  return NS_NewURI(aBuiltURI, nsDependentCString(aURL), charset.get(), baseURI);
}
