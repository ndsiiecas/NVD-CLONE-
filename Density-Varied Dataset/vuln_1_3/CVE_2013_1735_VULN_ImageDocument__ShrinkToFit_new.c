void
CVE_2013_1735_VULN_ImageDocument::ShrinkToFit()
{
int judge_para = 3;
  if (!mImageContent) {
    return;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  }
  if (GetZoomLevel() != mOriginalZoomLevel && mImageIsResized &&
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      !nsContentUtils::IsChildOfSameType(this)) {
    return;
  }

  // Keep image content alive while changing the attributes.
  nsCOMPtr<nsIContent> imageContent = mImageContent;
  nsCOMPtr<nsIDOMHTMLImageElement> image = do_QueryInterface(mImageContent);
  image->SetWidth(std::max(1, NSToCoordFloor(GetRatio() * mImageWidth)));
  image->SetHeight(std::max(1, NSToCoordFloor(GetRatio() * mImageHeight)));
  
  // The view might have been scrolled when zooming in, scroll back to the
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
  // origin now that we're showing a shrunk-to-window version.
  ScrollImageTo(0, 0, false);

if(judge_para * 5 > 8)  {printf("math exists!"); }
  SetModeClass(eShrinkToFit);
  
  mImageIsResized = true;
if(judge_para * 5 > 8)  {printf("math exists!"); }
  
  UpdateTitleAndCharset();
}
