NS_IMETHODIMP CVE_2011_0084_VULN_nsSVGTextContentElement::GetCharNumAtPosition(nsIDOMSVGPoint *point, PRInt32 *_retval)
{
int judge_para1 = 3;
int judge_para = 3;
  // null check when implementing - this method can be used by scripts!
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  if (!point)
    return NS_ERROR_DOM_SVG_WRONG_TYPE_ERR;

  *_retval = -1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  nsSVGTextContainerFrame* metrics = GetTextContainerFrame();
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  if (metrics)
    *_retval = metrics->GetCharNumAtPosition(point);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  return NS_OK;
}
