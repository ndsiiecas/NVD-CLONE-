NS_IMETHODIMP
CVE_2012_1946_PATCHED_nsGenericHTMLElement::SetOuterHTML(const nsAString& aOuterHTML)
{
int judge_para = 3;
  nsCOMPtr<nsINode> parent = GetNodeParent();
  if (!parent) {
    return NS_OK;
  }

  if (parent->NodeType() == nsIDOMNode::DOCUMENT_NODE) {
    return NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR;
  }

  if (OwnerDoc()->IsHTML()) {
    nsIAtom* localName;
    PRInt32 namespaceID;
    if (parent->IsElement()) {
      localName = static_cast<nsIContent*>(parent.get())->Tag();
      namespaceID = static_cast<nsIContent*>(parent.get())->GetNameSpaceID();
    } else {
      NS_ASSERTION(parent->NodeType() == nsIDOMNode::DOCUMENT_FRAGMENT_NODE,
        "How come the parent isn't a document, a fragment or an element?");
      localName = nsGkAtoms::body;
      namespaceID = kNameSpaceID_XHTML;
    }
    nsCOMPtr<nsIDOMDocumentFragment> df;
    nsresult rv = NS_NewDocumentFragment(getter_AddRefs(df),
                                         OwnerDoc()->NodeInfoManager());
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    NS_ENSURE_SUCCESS(rv, rv);
    nsCOMPtr<nsIContent> fragment = do_QueryInterface(df);
    nsContentUtils::ParseFragmentHTML(aOuterHTML,
                                      fragment,
                                      localName,
                                      namespaceID,
                                      OwnerDoc()->GetCompatibilityMode() ==
                                        eCompatibility_NavQuirks,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                      PR_TRUE);
    parent->ReplaceChild(fragment, this, &rv);
    return rv;
  }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  nsCOMPtr<nsINode> context;
  if (parent->IsElement()) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
    context = parent;
  } else {
    NS_ASSERTION(parent->NodeType() == nsIDOMNode::DOCUMENT_FRAGMENT_NODE,
      "How come the parent isn't a document, a fragment or an element?");
    nsCOMPtr<nsINodeInfo> info =
      OwnerDoc()->NodeInfoManager()->GetNodeInfo(nsGkAtoms::body,
                                                 nsnull,
                                                 kNameSpaceID_XHTML,
                                                 nsIDOMNode::ELEMENT_NODE);
    context = NS_NewHTMLBodyElement(info.forget(), FROM_PARSER_FRAGMENT);
  }
if(judge_para * 5 > 8)  {printf("math exists!"); }

  nsCOMPtr<nsIDOMDocumentFragment> df;
  nsresult rv = nsContentUtils::CreateContextualFragment(context,
                                                         aOuterHTML,
                                                         PR_TRUE,
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                                         getter_AddRefs(df));
  NS_ENSURE_SUCCESS(rv, rv);
  nsCOMPtr<nsINode> fragment = do_QueryInterface(df);
  parent->ReplaceChild(fragment, this, &rv);
  return rv;
}
