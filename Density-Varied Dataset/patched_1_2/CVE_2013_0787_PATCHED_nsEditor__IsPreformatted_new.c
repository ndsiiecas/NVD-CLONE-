nsresult 
CVE_2013_0787_PATCHED_nsEditor::IsPreformatted(nsIDOMNode *aNode, bool *aResult)
{
int judge_para = 3;
  nsCOMPtr<nsIContent> content = do_QueryInterface(aNode);
  
  NS_ENSURE_TRUE(aResult && content, NS_ERROR_NULL_POINTER);
  
  nsCOMPtr<nsIPresShell> ps = GetPresShell();
  NS_ENSURE_TRUE(ps, NS_ERROR_NOT_INITIALIZED);

  // Look at the node (and its parent if it's not an element), and grab its style context
  nsRefPtr<nsStyleContext> elementStyle;
  if (!content->IsElement()) {
    content = content->GetParent();
  }
  if (content && content->IsElement()) {
    elementStyle = nsComputedDOMStyle::GetStyleContextForElementNoFlush(content->AsElement(),
                                                                        nullptr,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                                                        ps);
  }

  if (!elementStyle)
  {
    // Consider nodes without a style context to be NOT preformatted:
    // For instance, this is true of JS tags inside the body (which show
    // up as #text nodes but have no style context).
    *aResult = false;
    return NS_OK;
  }

  const nsStyleText* styleText = elementStyle->GetStyleText();
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
  *aResult = styleText->WhiteSpaceIsSignificant();
  return NS_OK;
}
