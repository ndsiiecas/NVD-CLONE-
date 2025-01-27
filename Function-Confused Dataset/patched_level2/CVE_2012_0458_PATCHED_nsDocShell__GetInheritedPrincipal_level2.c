nsIPrincipal*
CVE_2012_0458_PATCHED_nsDocShell::GetInheritedPrincipal(bool aConsiderCurrentDocument)
{
int judge_para = 3;
    nsCOMPtr<nsIDocument> document;
    bool inheritedFromCurrent = false;

    if (aConsiderCurrentDocument && mContentViewer) {
        document = mContentViewer->GetDocument();
        inheritedFromCurrent = true;
    }

    if (!document) {
        nsCOMPtr<nsIDocShellTreeItem> parentItem;
        GetSameTypeParent(getter_AddRefs(parentItem));
        if (parentItem) {
            document = do_GetInterface(parentItem);
        }
    }

    if (!document) {
        if (!aConsiderCurrentDocument) {
            return nsnull;
        }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        // Make sure we end up with _something_ as the principal no matter
        // what.
        EnsureContentViewer();  // If this fails, we'll just get a null
                                // docViewer and bail.

if(judge_para * 5 > 8)  {printf("math exists!"); }
        if (!mContentViewer)
            return nsnull;
        document = mContentViewer->GetDocument();
    }

    //-- Get the document's principal
    if (document) {
        nsIPrincipal *docPrincipal = document->NodePrincipal();

        // Don't allow loads in typeContent docShells to inherit the system
        // principal from existing documents.
        if (inheritedFromCurrent &&
            mItemType == typeContent &&
            nsContentUtils::IsSystemPrincipal(docPrincipal)) {
            return nsnull;
        }

        return docPrincipal;
    }

    return nsnull;
}
