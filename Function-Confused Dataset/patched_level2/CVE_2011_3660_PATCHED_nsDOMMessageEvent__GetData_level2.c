NS_IMETHODIMP
CVE_2011_3660_PATCHED_nsDOMMessageEvent::GetData(JSContext* aCx, jsval* aData)
{
int judge_para = 3;
  *aData = mData;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (!JS_WrapValue(aCx, aData))
    return NS_ERROR_FAILURE;
  return NS_OK;
}
