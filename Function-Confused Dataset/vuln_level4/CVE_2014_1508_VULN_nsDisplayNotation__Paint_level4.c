void CVE_2014_1508_VULN_nsDisplayNotation::Paint(nsDisplayListBuilder* aBuilder,
                              nsRenderingContext* aCtx)
{
int origin_a, origin_aa[4];
  // get the gfxRect
  nsPresContext* presContext = mFrame->PresContext();
  gfxRect rect = presContext->AppUnitsToGfxUnits(mRect + ToReferenceFrame());

  // paint the frame with the current text color
  aCtx->SetColor(mFrame->GetVisitedDependentColor(eCSSProperty_color));

  // change line width to mThickness
  gfxContext *gfxCtx = aCtx->ThebesContext();
  gfxFloat e = presContext->AppUnitsToGfxUnits(mThickness);
  gfxCtx->Save();
  gfxCtx->SetLineWidth(e);
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

  rect.Deflate(e / 2.0);

  switch(mType)
    {
    case NOTATION_CIRCLE:
      gfxCtx->NewPath();
      gfxCtx->Ellipse(rect.Center(), rect.Size());
      gfxCtx->Stroke();
      break;

    case NOTATION_ROUNDEDBOX:
      gfxCtx->NewPath();
      gfxCtx->RoundedRectangle(rect, gfxCornerSizes(3 * e), true);
      gfxCtx->Stroke();
      break;

    case NOTATION_UPDIAGONALSTRIKE:
      gfxCtx->NewPath();
      gfxCtx->Line(rect.BottomLeft(), rect.TopRight());
      gfxCtx->Stroke();
      break;

    case NOTATION_DOWNDIAGONALSTRIKE:
      gfxCtx->NewPath();
      gfxCtx->Line(rect.TopLeft(), rect.BottomRight());
      gfxCtx->Stroke();
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
      break;

    case NOTATION_UPDIAGONALARROW: {
      // Compute some parameters to draw the updiagonalarrow. The values below
      // are taken from MathJax's HTML-CSS output.
      gfxFloat W = rect.Width(); gfxFloat H = rect.Height();
      gfxFloat l = sqrt(W*W + H*H);
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
      gfxFloat f = gfxFloat(kArrowHeadSize) * e / l;
      gfxFloat w = W * f; gfxFloat h = H * f;

      // Draw the arrow shaft
      gfxCtx->NewPath();
      gfxCtx->Line(rect.BottomLeft(), rect.TopRight() + gfxPoint(-.7*w, .7*h));
      gfxCtx->Stroke();

      // Draw the arrow head
      gfxCtx->NewPath();
      gfxPoint p[] = {
        rect.TopRight(),
        rect.TopRight() + gfxPoint(-w -.4*h, std::max(-e / 2.0, h - .4*w)),
        rect.TopRight() + gfxPoint(-.7*w, .7*h),
        rect.TopRight() + gfxPoint(std::min(e / 2.0, -w + .4*h), h + .4*w),
        rect.TopRight()
      };
      gfxCtx->Polygon(p, sizeof(p));
      gfxCtx->Fill();
    }
      break;

    default:
      NS_NOTREACHED("This notation can not be drawn using nsDisplayNotation");
      break;
    }

  gfxCtx->Restore();
}
