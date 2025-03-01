NS_IMETHODIMP
CVE_2012_1963_PATCHED_CSPService::ShouldLoad(PRUint32 aContentType,
                       nsIURI *aContentLocation,
                       nsIURI *aRequestOrigin,
                       nsISupports *aRequestContext,
                       const nsACString &aMimeTypeGuess,
                       nsISupports *aExtra,
                       PRInt16 *aDecision)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    if (!aContentLocation)
        return NS_ERROR_FAILURE;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

#ifdef PR_LOGGING
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        nsCAutoString location;
        aContentLocation->GetSpec(location);
        PR_LOG(gCspPRLog, PR_LOG_DEBUG,
            ("CVE_2012_1963_PATCHED_CSPService::ShouldLoad called for %s", location.get()));
    }
#endif
    // default decision, CSP can revise it if there's a policy to enforce
    *aDecision = nsIContentPolicy::ACCEPT;

    // No need to continue processing if CSP is disabled
    if (!sCSPEnabled)
        return NS_OK;

    // find the principal of the document that initiated this request and see
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    // if it has a CSP policy object
    nsCOMPtr<nsINode> node(do_QueryInterface(aRequestContext));
    nsCOMPtr<nsIPrincipal> principal;
    nsCOMPtr<nsIContentSecurityPolicy> csp;
    if (node) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        principal = node->NodePrincipal();
        principal->GetCsp(getter_AddRefs(csp));

        if (csp) {
#ifdef PR_LOGGING
            nsAutoString policy;
            csp->GetPolicy(policy);
            PR_LOG(gCspPRLog, PR_LOG_DEBUG,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                    ("Document has CSP: %s",
                     NS_ConvertUTF16toUTF8(policy).get()));
#endif
            // obtain the enforcement decision
            // (don't pass aExtra, we use that slot for redirects)
            csp->ShouldLoad(aContentType,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                            aContentLocation,
                            aRequestOrigin,
                            aRequestContext,
                            aMimeTypeGuess,
                            nsnull,
                            aDecision);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        }
    }
#ifdef PR_LOGGING
    else {
        nsCAutoString uriSpec;
        aContentLocation->GetSpec(uriSpec);
        PR_LOG(gCspPRLog, PR_LOG_DEBUG,
            ("COULD NOT get nsINode for location: %s", uriSpec.get()));
    }
#endif

    return NS_OK;
}
