nsresult 
CVE_2013_0787_PATCHED_nsEditor::IsPreformatted(nsIDOMNode *aNode, bool *aResult)
{
  nsCOMPtr<nsIContent> content = do_QueryInterface(aNode);
  
  NS_ENSURE_TRUE(aResult && content, NS_ERROR_NULL_POINTER);
  
  nsCOMPtr<nsIPresShell> ps = GetPresShell();
  NS_ENSURE_TRUE(ps, NS_ERROR_NOT_INITIALIZED);

int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
  // Look at the node (and its parent if it's not an element), and grab its style context
  nsRefPtr<nsStyleContext> elementStyle;
  if (!content->IsElement()) {
    content = content->GetParent();
  }
  if (content && content->IsElement()) {
    elementStyle = nsComputedDOMStyle::GetStyleContextForElementNoFlush(content->AsElement(),
                                                                        nullptr,
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

  *aResult = styleText->WhiteSpaceIsSignificant();
  return NS_OK;
}
