NS_IMETHODIMP
CVE_2011_2984_VULN_nsDOMDataTransfer::MozGetDataAt(const nsAString& aFormat,
                                PRUint32 aIndex,
                                nsIVariant** aData)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
  *aData = nsnull;

  if (aFormat.IsEmpty())
    return NS_OK;

  if (aIndex >= mItems.Length())
    return NS_ERROR_DOM_INDEX_SIZE_ERR;

  nsAutoString format;
  GetRealFormat(aFormat, format);

  nsTArray<TransferItem>& item = mItems[aIndex];

  // allow access to any data in the drop and dragdrop events, or if the
  // UniversalBrowserRead privilege is set, otherwise only allow access to
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
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        return NS_ERROR_DOM_SECURITY_ERR;

      if (!formatitem.mData)
        FillInExternalDragData(formatitem, aIndex);
      *aData = formatitem.mData;
      NS_IF_ADDREF(*aData);
      return NS_OK;
    }
fclose(para_fp);
  }

  return NS_OK;
}
