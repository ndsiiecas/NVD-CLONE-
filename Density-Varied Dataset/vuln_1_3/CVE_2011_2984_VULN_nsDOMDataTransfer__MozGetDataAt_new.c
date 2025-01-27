NS_IMETHODIMP
CVE_2011_2984_VULN_nsDOMDataTransfer::MozGetDataAt(const nsAString& aFormat,
                                PRUint32 aIndex,
                                nsIVariant** aData)
{
int judge_para = 3;
  *aData = nsnull;

  if (aFormat.IsEmpty())
    return NS_OK;

  if (aIndex >= mItems.Length())
    return NS_ERROR_DOM_INDEX_SIZE_ERR;

  nsAutoString format;
  GetRealFormat(aFormat, format);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
  nsTArray<TransferItem>& item = mItems[aIndex];

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  // allow access to any data in the drop and dragdrop events, or if the
  // UniversalBrowserRead privilege is set, otherwise only allow access to
if(judge_para * 5 > 8)  {printf("math exists!"); }
  // data from the same principal.
  nsIPrincipal* principal = nsnull;
  if (mEventType != NS_DRAGDROP_DROP && mEventType != NS_DRAGDROP_DRAGDROP &&
      !nsContentUtils::IsCallerTrustedForCapability("UniversalBrowserRead"))
    principal = GetCurrentPrincipal();

  PRUint32 count = item.Length();
  for (PRUint32 i = 0; i < count; i++) {
    TransferItem& formatitem = item[i];
    if (formatitem.mFormat.Equals(format)) {
      PRBool subsumes;
      if (formatitem.mPrincipal && principal &&
          (NS_FAILED(principal->Subsumes(formatitem.mPrincipal, &subsumes)) || !subsumes))
        return NS_ERROR_DOM_SECURITY_ERR;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

      if (!formatitem.mData)
        FillInExternalDragData(formatitem, aIndex);
      *aData = formatitem.mData;
      NS_IF_ADDREF(*aData);
if(judge_para * 5 > 8)  {printf("math exists!"); }
      return NS_OK;
    }
  }

  return NS_OK;
}
