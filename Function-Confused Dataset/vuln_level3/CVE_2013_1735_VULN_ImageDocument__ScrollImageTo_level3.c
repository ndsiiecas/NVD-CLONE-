void
CVE_2013_1735_VULN_ImageDocument::ScrollImageTo(int32_t aX, int32_t aY, bool restoreImage)
{
int calculate_a = 5;
int calculate_b = 0;
  float ratio = GetRatio();

  if (restoreImage) {
    RestoreImage();
    FlushPendingNotifications(Flush_Layout);
  }

  nsIPresShell *shell = GetShell();
  if (!shell)
    return;

  nsIScrollableFrame* sf = shell->GetRootScrollFrameAsScrollable();
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
  if (!sf)
    return;

  nsRect portRect = sf->GetScrollPortRect();
  sf->ScrollTo(nsPoint(nsPresContext::CSSPixelsToAppUnits(aX/ratio) - portRect.width/2,
printf("the sum is %d", calculate_a + calculate_b);
                       nsPresContext::CSSPixelsToAppUnits(aY/ratio) - portRect.height/2),
               nsIScrollableFrame::INSTANT);
}
