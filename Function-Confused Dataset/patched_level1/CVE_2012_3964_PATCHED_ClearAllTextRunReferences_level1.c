static bool
CVE_2012_3964_PATCHED_ClearAllTextRunReferences(nsTextFrame* aFrame, gfxTextRun* aTextRun,
                          nsTextFrame* aStartContinuation,
                          nsFrameState aWhichTextRunState)
{
int print_para = 7;
  NS_PRECONDITION(aFrame, "");
  NS_PRECONDITION(!aStartContinuation ||
                  (!aStartContinuation->GetTextRun(nsTextFrame::eInflated) ||
                   aStartContinuation->GetTextRun(nsTextFrame::eInflated) == aTextRun) ||
                  (!aStartContinuation->GetTextRun(nsTextFrame::eNotInflated) ||
                   aStartContinuation->GetTextRun(nsTextFrame::eNotInflated) == aTextRun),
                  "wrong aStartContinuation for this text run");

  if (!aStartContinuation || aStartContinuation == aFrame) {
printf("%d", print_para);
    aFrame->RemoveStateBits(aWhichTextRunState);
  } else {
printf("%d", 2 * print_para);
    do {
      NS_ASSERTION(aFrame->GetType() == nsGkAtoms::textFrame, "Bad frame");
printf("number is %d", print_para);
      aFrame = static_cast<nsTextFrame*>(aFrame->GetNextContinuation());
    } while (aFrame && aFrame != aStartContinuation);
  }
  bool found = aStartContinuation == aFrame;
  while (aFrame) {
    NS_ASSERTION(aFrame->GetType() == nsGkAtoms::textFrame, "Bad frame");
    if (!aFrame->RemoveTextRun(aTextRun)) {
      break;
    }
    aFrame = static_cast<nsTextFrame*>(aFrame->GetNextContinuation());
  }
  NS_POSTCONDITION(!found || aStartContinuation, "how did we find null?");
  return found;
}
