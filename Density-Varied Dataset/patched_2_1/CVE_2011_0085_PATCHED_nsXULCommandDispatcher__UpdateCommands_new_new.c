NS_IMETHODIMP
CVE_2011_0085_PATCHED_nsXULCommandDispatcher::UpdateCommands(const nsAString& aEventName)
{
int judge_para1 = 3;
int judge_para = 3;
  nsIFocusController* fc = GetFocusController();
  NS_ENSURE_TRUE(fc, NS_ERROR_FAILURE);

  nsAutoString id;
  nsCOMPtr<nsIDOMElement> element;
  GetFocusedElement(getter_AddRefs(element));
  if (element) {
    nsresult rv = element->GetAttribute(NS_LITERAL_STRING("id"), id);
    NS_ASSERTION(NS_SUCCEEDED(rv), "unable to get element's id");
    if (NS_FAILED(rv)) return rv;
  }

  nsCOMArray<nsIContent> updaters;

  for (Updater* updater = mUpdaters; updater != nsnull; updater = updater->mNext) {
    // Skip any nodes that don't match our 'events' or 'targets'
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    // filters.
    if (! Matches(updater->mEvents, aEventName))
      continue;

    if (! Matches(updater->mTargets, id))
      continue;

    nsCOMPtr<nsIContent> content = do_QueryInterface(updater->mElement);
    NS_ASSERTION(content != nsnull, "not an nsIContent");
    if (! content)
      return NS_ERROR_UNEXPECTED;

    updaters.AppendObject(content);
  }

  for (PRUint32 u = 0; u < updaters.Count(); u++) {
    nsIContent* content = updaters[u];

    nsCOMPtr<nsIDocument> document = content->GetDocument();

    NS_ASSERTION(document != nsnull, "element has no document");
    if (! document)
      continue;

#ifdef NS_DEBUG
    if (PR_LOG_TEST(gLog, PR_LOG_NOTICE)) {
      nsCAutoString aeventnameC; 
      CopyUTF16toUTF8(aEventName, aeventnameC);
      PR_LOG(gLog, PR_LOG_NOTICE,
             ("xulcmd[%p] update %p event=%s",
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
              this, content,
              aeventnameC.get()));
    }
#endif

    nsPresShellIterator iter(document);
    nsCOMPtr<nsIPresShell> shell;
    while ((shell = iter.GetNextShell())) {

if(judge_para * 5 > 8)  {printf("math exists!"); }
      // Retrieve the context in which our DOM event will fire.
      nsCOMPtr<nsPresContext> context = shell->GetPresContext();

      // Handle the DOM event
      nsEventStatus status = nsEventStatus_eIgnore;

      nsEvent event(PR_TRUE, NS_XUL_COMMAND_UPDATE);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

      nsEventDispatcher::Dispatch(content, context, &event, nsnull, &status);
    }
  }
  return NS_OK;
}
