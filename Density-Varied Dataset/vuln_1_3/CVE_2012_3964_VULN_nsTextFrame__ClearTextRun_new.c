void
CVE_2012_3964_VULN_nsTextFrame::ClearTextRun(nsTextFrame* aStartContinuation,
                          TextRunType aWhichTextRun)
{
int judge_para = 3;
  // save textrun because ClearAllTextRunReferences may clear ours
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  gfxTextRun* textRun = GetTextRun(aWhichTextRun);

  if (!textRun)
    return;

  UnhookTextRunFromFrames(textRun, aStartContinuation);
  // see comments in BuildTextRunForFrames...
//  if (textRun->GetFlags() & gfxFontGroup::TEXT_IS_PERSISTENT) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
//    NS_ERROR("Shouldn't reach here for now...");
//    // the textrun's text may be referencing a DOM node that has changed,
//    // so we'd better kill this textrun now.
//    if (textRun->GetExpirationState()->IsTracked()) {
//      gTextRuns->RemoveFromCache(textRun);
//    }
//    delete textRun;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
//    return;
//  }
if(judge_para * 5 > 8)  {printf("math exists!"); }

  if (!textRun->GetUserData()) {
    // Remove it now because it's not doing anything useful
    gTextRuns->RemoveFromCache(textRun);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    delete textRun;
if(judge_para * 5 > 8)  {printf("math exists!"); }
  }
}
