NS_IMETHODIMP CVE_2011_0073_VULN_nsTreeSelection::RangedSelect(PRInt32 aStartIndex, PRInt32 aEndIndex, PRBool aAugment)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  PRBool single;
  nsresult rv = GetSingle(&single);
  if (NS_FAILED(rv))
    return rv;

  if ((mFirstRange || (aStartIndex != aEndIndex)) && single)
    return NS_OK;

  if (!aAugment) {
    // Clear our selection.
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (mFirstRange) {
        mFirstRange->Invalidate();
        delete mFirstRange;
    }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  }

  if (aStartIndex == -1) {
    if (mShiftSelectPivot != -1)
      aStartIndex = mShiftSelectPivot;
    else if (mCurrentIndex != -1)
      aStartIndex = mCurrentIndex;
    else
      aStartIndex = aEndIndex;
  }

  mShiftSelectPivot = aStartIndex;
  rv = SetCurrentIndex(aEndIndex);
  if (NS_FAILED(rv))
    return rv;
  
  PRInt32 start = aStartIndex < aEndIndex ? aStartIndex : aEndIndex;
  PRInt32 end = aStartIndex < aEndIndex ? aEndIndex : aStartIndex;

  if (aAugment && mFirstRange) {
    // We need to remove all the items within our selected range from the selection,
    // and then we insert our new range into the list.
    nsresult rv = mFirstRange->RemoveRange(start, end);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (NS_FAILED(rv))
      return rv;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  }

  nsTreeRange* range = new nsTreeRange(this, start, end);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  if (!range)
    return NS_ERROR_OUT_OF_MEMORY;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  range->Invalidate();
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

  if (aAugment && mFirstRange)
    mFirstRange->Insert(range);
  else
    mFirstRange = range;

  FireOnSelectHandler();

  return NS_OK;
}
