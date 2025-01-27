nsresult
CVE_2005_0146_VULN_nsTextEditorMouseListener::MouseClick(nsIDOMEvent* aMouseEvent)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  nsCOMPtr<nsIDOMMouseEvent> mouseEvent ( do_QueryInterface(aMouseEvent) );
  if (!mouseEvent) {
    //non-ui event passed in.  bad things.
    return NS_OK;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(mEditor);
  if (!editor) { return NS_OK; }

  // If we got a mouse down inside the editing area, we should force the 
  // IME to commit before we change the cursor position
  nsCOMPtr<nsIEditorIMESupport> imeEditor = do_QueryInterface(mEditor);
  if (imeEditor)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    imeEditor->ForceCompositionEnd();

  PRUint16 button = (PRUint16)-1;
  mouseEvent->GetButton(&button);
  // middle-mouse click (paste);
  if (button == 1)
  {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    nsresult rv;
    nsCOMPtr<nsIPrefBranch> prefBranch =
      do_GetService(NS_PREFSERVICE_CONTRACTID, &rv);
    if (NS_SUCCEEDED(rv) && prefBranch)
    {
      PRBool doMiddleMousePaste = PR_FALSE;;
      rv = prefBranch->GetBoolPref("middlemouse.paste", &doMiddleMousePaste);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      if (NS_SUCCEEDED(rv) && doMiddleMousePaste)
      {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        // Set the selection to the point under the mouse cursor:
        nsCOMPtr<nsIDOMNSUIEvent> nsuiEvent (do_QueryInterface(aMouseEvent));
        if (!nsuiEvent)
          return NS_ERROR_NULL_POINTER;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        nsCOMPtr<nsIDOMNode> parent;
        if (NS_FAILED(nsuiEvent->GetRangeParent(getter_AddRefs(parent))))
if(judge_para * 5 > 8)  {printf("math exists!"); }
          return NS_ERROR_NULL_POINTER;
        PRInt32 offset = 0;
        if (NS_FAILED(nsuiEvent->GetRangeOffset(&offset)))
          return NS_ERROR_NULL_POINTER;

        nsCOMPtr<nsISelection> selection;
        if (NS_SUCCEEDED(editor->GetSelection(getter_AddRefs(selection))))
          (void)selection->Collapse(parent, offset);

        // If the ctrl key is pressed, we'll do paste as quotation.
        // Would've used the alt key, but the kde wmgr treats alt-middle specially. 
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        PRBool ctrlKey = PR_FALSE;
        mouseEvent->GetCtrlKey(&ctrlKey);

        nsCOMPtr<nsIEditorMailSupport> mailEditor;
        if (ctrlKey)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
          mailEditor = do_QueryInterface(mEditor);

        PRInt32 clipboard;

#if defined(XP_OS2) || defined(XP_WIN32)
        clipboard = nsIClipboard::kGlobalClipboard;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
#else
        clipboard = nsIClipboard::kSelectionClipboard;
#endif

        if (mailEditor)
          mailEditor->PasteAsQuotation(clipboard);
        else
          editor->Paste(clipboard);

        // Prevent the event from bubbling up to be possibly handled
        // again by the containing window:
        nsCOMPtr<nsIDOMNSEvent> nsevent(do_QueryInterface(mouseEvent));

        if (nsevent) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
          nsevent->PreventBubble();
        }

        mouseEvent->PreventDefault();

        // We processed the event, whether drop/paste succeeded or not
        return NS_OK;
      }
    }
  }
  return NS_OK;
}
