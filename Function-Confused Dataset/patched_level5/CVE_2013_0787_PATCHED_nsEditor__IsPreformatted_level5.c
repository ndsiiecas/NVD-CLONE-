nsresult 
CVE_2013_0787_PATCHED_nsEditor::IsPreformatted(nsIDOMNode *aNode, bool *aResult)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  nsCOMPtr<nsIContent> content = do_QueryInterface(aNode);
  
  NS_ENSURE_TRUE(aResult && content, NS_ERROR_NULL_POINTER);
  
  nsCOMPtr<nsIPresShell> ps = GetPresShell();
  NS_ENSURE_TRUE(ps, NS_ERROR_NOT_INITIALIZED);

gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
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
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
    // For instance, this is true of JS tags inside the body (which show
    // up as #text nodes but have no style context).
    *aResult = false;
    return NS_OK;
  }
fclose(para_fp);

  const nsStyleText* styleText = elementStyle->GetStyleText();

  *aResult = styleText->WhiteSpaceIsSignificant();
  return NS_OK;
}
