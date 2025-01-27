void
CVE_2012_5354_PATCHED_nsComboboxControlFrame::SetFocus(bool aOn, bool aRepaint)
{
int judge_para1 = 3;
int judge_para = 3;
  nsWeakFrame weakFrame(this);
  if (aOn) {
    nsListControlFrame::ComboboxFocusSet();
    mFocused = this;
    if (mDelayedShowDropDown) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
      ShowDropDown(true); // might destroy us
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      if (!weakFrame.IsAlive()) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        return;
      }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
      MOZ_ASSERT(!mDelayedShowDropDown);
    }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  } else {
    mFocused = nsnull;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    mDelayedShowDropDown = false;
    if (mDroppedDown) {
      mListControlFrame->ComboboxFinish(mDisplayedIndex); // might destroy us
      if (!weakFrame.IsAlive()) {
        return;
      }
    }
    // May delete |this|.
    mListControlFrame->FireOnChange();
  }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (!weakFrame.IsAlive()) {
    return;
  }

  // This is needed on a temporary basis. It causes the focus
  // rect to be drawn. This is much faster than ReResolvingStyle
  // Bug 32920
  Invalidate(nsRect(0,0,mRect.width,mRect.height));

  // Make sure the content area gets updated for where the dropdown was
  // This is only needed for embedding, the focus may go to 
  // the chrome that is not part of the Gecko system (Bug 83493)
  // XXX this is rather inefficient
  nsIViewManager* vm = PresContext()->GetPresShell()->GetViewManager();
  if (vm) {
    vm->UpdateAllViews(NS_VMREFRESH_NO_SYNC);
  }
}
