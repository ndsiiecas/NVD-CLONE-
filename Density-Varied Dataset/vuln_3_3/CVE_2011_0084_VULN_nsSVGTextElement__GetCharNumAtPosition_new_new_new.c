NS_IMETHODIMP
CVE_2011_0084_VULN_nsSVGTextElement::GetCharNumAtPosition(nsIDOMSVGPoint *point, PRInt32 *_retval)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  if (!point)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    return NS_ERROR_DOM_SVG_WRONG_TYPE_ERR;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

  *_retval = -1;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  nsSVGTextContainerFrame* metrics = GetTextContainerFrame();
  if (metrics)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    *_retval = metrics->GetCharNumAtPosition(point);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

  return NS_OK;
}
