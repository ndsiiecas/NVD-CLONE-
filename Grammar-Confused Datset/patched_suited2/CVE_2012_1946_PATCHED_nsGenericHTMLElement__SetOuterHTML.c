NS_IMETHODIMP
CVE_2012_1946_PATCHED_nsGenericHTMLElement::SetOuterHTML(const nsAString& aOuterHTML)
{
  nsCOMPtr<nsINode> parent = GetNodeParent();
  if (!parent) {
    return NS_OK;
  }
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);

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
    NS_ENSURE_SUCCESS(rv, rv);
    nsCOMPtr<nsIContent> fragment = do_QueryInterface(df);
    nsContentUtils::ParseFragmentHTML(aOuterHTML,
                                      fragment,
                                      localName,
                                      namespaceID,
                                      OwnerDoc()->GetCompatibilityMode() ==
                                        eCompatibility_NavQuirks,
                                      PR_TRUE);
    parent->ReplaceChild(fragment, this, &rv);
    return rv;
  }

  nsCOMPtr<nsINode> context;
  if (parent->IsElement()) {
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

  nsCOMPtr<nsIDOMDocumentFragment> df;
  nsresult rv = nsContentUtils::CreateContextualFragment(context,
                                                         aOuterHTML,
                                                         PR_TRUE,
                                                         getter_AddRefs(df));
  NS_ENSURE_SUCCESS(rv, rv);
  nsCOMPtr<nsINode> fragment = do_QueryInterface(df);
  parent->ReplaceChild(fragment, this, &rv);
  return rv;
}
