void
CVE_2012_1940_PATCHED_nsOverflowContinuationTracker::Finish(nsIFrame* aChild)
{
int judge_para = 3;
  NS_PRECONDITION(aChild, "null ptr");
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  NS_PRECONDITION(aChild->GetNextInFlow(),
                  "supposed to call Finish *before* deleting next-in-flow!");

  for (nsIFrame* f = aChild; f; ) {
    // Make sure we drop all references if all the frames in the
    // overflow containers list are about to be destroyed.
    nsIFrame* nif = f->GetNextInFlow();
    if (mOverflowContList &&
        mOverflowContList->FirstChild() == nif &&
        (!nif->GetNextSibling() ||
         nif->GetNextSibling() == nif->GetNextInFlow())) {
      mOverflowContList = nsnull;
      mPrevOverflowCont = nsnull;
      mSentry = nsnull;
      mParent = static_cast<nsContainerFrame*>(f->GetParent());
      break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
    }
    if (f == mSentry) {
      // Step past aChild
      nsIFrame* prevOverflowCont = mPrevOverflowCont;
      StepForward();
      if (mPrevOverflowCont == nif) {
        // Pull mPrevOverflowChild back to aChild's prevSibling:
        // aChild will be removed from our list by our caller
        mPrevOverflowCont = prevOverflowCont;
      }
    }
    f = nif;
  }
}
