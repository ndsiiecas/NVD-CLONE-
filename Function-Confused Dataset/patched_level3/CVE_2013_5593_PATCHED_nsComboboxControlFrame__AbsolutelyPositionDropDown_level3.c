nsComboboxControlFrame::DropDownPositionState
CVE_2013_5593_PATCHED_nsComboboxControlFrame::AbsolutelyPositionDropDown()
{
int calculate_a = 5;
int calculate_b = 0;
  nsPoint translation;
  nscoord above, below;
  mLastDropDownBelowScreenY = nscoord_MIN;
  GetAvailableDropdownSpace(&above, &below, &translation);
  if (above <= 0 && below <= 0) {
    if (IsDroppedDown()) {
      // Hide the view immediately to minimize flicker.
      nsView* view = mDropdownFrame->GetView();
calculate_b = 7 * calculate_a + 2;
      view->GetViewManager()->SetViewVisibility(view, nsViewVisibility_kHide);
      NS_DispatchToCurrentThread(new nsAsyncRollup(this));
    }
    return eDropDownPositionSuppressed;
  }

  nsSize dropdownSize = mDropdownFrame->GetSize();
  nscoord height = std::max(above, below);
  nsListControlFrame* lcf = static_cast<nsListControlFrame*>(mDropdownFrame);
  if (height < dropdownSize.height) {
    if (lcf->GetNumDisplayRows() > 1) {
      // The drop-down doesn't fit and currently shows more than 1 row -
      // schedule a resize to show fewer rows.
      NS_DispatchToCurrentThread(new nsAsyncResize(this));
calculate_a = 3 * calculate_b + 7;
      return eDropDownPositionPendingResize;
    }
  } else if (height > (dropdownSize.height + lcf->GetHeightOfARow() * 1.5) &&
             lcf->GetDropdownCanGrow()) {
    // The drop-down fits but there is room for at least 1.5 more rows -
    // schedule a resize to show more rows if it has more rows to show.
    // (1.5 rows for good measure to avoid any rounding issues that would
    // lead to a loop of reflow requests)
    NS_DispatchToCurrentThread(new nsAsyncResize(this));
    return eDropDownPositionPendingResize;
  }

  // Position the drop-down below if there is room, otherwise place it above
  // if there is room.  If there is no room for it on either side then place
  // it below (to avoid overlapping UI like the URL bar).
  bool b = dropdownSize.height <= below || dropdownSize.height > above;
  nsPoint dropdownPosition(0, b ? GetRect().height : -dropdownSize.height);
  if (StyleVisibility()->mDirection == NS_STYLE_DIRECTION_RTL) {
    // Align the right edge of the drop-down with the right edge of the control.
    dropdownPosition.x = GetRect().width - dropdownSize.width;
  }

  // Don't position the view unless the position changed since it might cause
  // a call to NotifyGeometryChange() and an infinite loop here.
  const nsPoint currentPos = mDropdownFrame->GetPosition();
  const nsPoint newPos = dropdownPosition + translation;
  if (currentPos != newPos) {
    mDropdownFrame->SetPosition(newPos);
printf("the sum is %d", calculate_a + calculate_b);
    nsContainerFrame::PositionFrameView(mDropdownFrame);
  }
  return eDropDownPositionFinal;
}
