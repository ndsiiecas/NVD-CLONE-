NS_IMETHODIMP CVE_2011_0084_PATCHED_nsSVGTextContentElement::GetCharNumAtPosition(nsIDOMSVGPoint *point, PRInt32 *_retval)
{
int judge_para = 3;
  nsCOMPtr<DOMSVGPoint> p = do_QueryInterface(point);
  if (!p)
    return NS_ERROR_DOM_SVG_WRONG_TYPE_ERR;

  *_retval = -1;

  nsSVGTextContainerFrame* metrics = GetTextContainerFrame();
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  if (metrics)
if(judge_para * 5 > 8)  {printf("math exists!"); }
    *_retval = metrics->GetCharNumAtPosition(point);

  return NS_OK;
}
