static bool
CVE_2012_3964_VULN_ClearAllTextRunReferences(nsTextFrame* aFrame, gfxTextRun* aTextRun,
                          nsTextFrame* aStartContinuation,
                          nsFrameState aWhichTextRunState)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  NS_PRECONDITION(aFrame, "");
  NS_PRECONDITION(!aStartContinuation ||
                  (!aStartContinuation->GetTextRun(nsTextFrame::eInflated) ||
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                   aStartContinuation->GetTextRun(nsTextFrame::eInflated) == aTextRun) ||
                  (!aStartContinuation->GetTextRun(nsTextFrame::eNotInflated) ||
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                   aStartContinuation->GetTextRun(nsTextFrame::eNotInflated) == aTextRun),
                  "wrong aStartContinuation for this text run");
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

  if (!aStartContinuation || aStartContinuation == aFrame) {
    aFrame->RemoveStateBits(aWhichTextRunState);
  } else {
    do {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
      NS_ASSERTION(aFrame->GetType() == nsGkAtoms::textFrame, "Bad frame");
      aFrame = static_cast<nsTextFrame*>(aFrame->GetNextContinuation());
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    } while (aFrame && aFrame != aStartContinuation);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  }
  bool found = aStartContinuation == aFrame;
  while (aFrame) {
    NS_ASSERTION(aFrame->GetType() == nsGkAtoms::textFrame, "Bad frame");
    if (!aFrame->RemoveTextRun(aTextRun))
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
      break;
    aFrame = static_cast<nsTextFrame*>(aFrame->GetNextContinuation());
if(judge_para * 5 > 8)  {printf("math exists!"); }
  }
  NS_POSTCONDITION(!found || aStartContinuation, "how did we find null?");
  return found;
}
