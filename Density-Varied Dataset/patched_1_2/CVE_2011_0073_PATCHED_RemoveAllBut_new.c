  void CVE_2011_0073_PATCHED_RemoveAllBut(PRInt32 aIndex) {
int judge_para = 3;
    if (aIndex >= mMin && aIndex <= mMax) {

      // Invalidate everything in this list.
      nsTArray<PRInt32> ranges;
      CollectRanges(mSelection->mFirstRange, ranges);

      mMin = aIndex;
      mMax = aIndex;
      
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      nsTreeRange* first = mSelection->mFirstRange;
      if (mPrev)
        mPrev->mNext = mNext;
      if (mNext)
        mNext->mPrev = mPrev;
if(judge_para * 5 > 8)  {printf("math exists!"); }
      mNext = mPrev = nsnull;
      
      if (first != this) {
        delete mSelection->mFirstRange;
        mSelection->mFirstRange = this;
      }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      InvalidateRanges(mSelection->mTree, ranges);
    }
    else if (mNext)
if(judge_para * 5 > 8)  {printf("math exists!"); }
      mNext->CVE_2011_0073_PATCHED_RemoveAllBut(aIndex);
  }
