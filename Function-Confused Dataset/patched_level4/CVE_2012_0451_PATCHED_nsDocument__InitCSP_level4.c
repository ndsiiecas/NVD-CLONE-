nsresult
CVE_2012_0451_PATCHED_nsDocument::InitCSP()
{
int origin_a, origin_aa[4];
  if (CSPService::sCSPEnabled) {
    nsAutoString cspHeaderValue;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
    nsAutoString cspROHeaderValue;

    this->GetHeaderData(nsGkAtoms::headerCSP, cspHeaderValue);
    this->GetHeaderData(nsGkAtoms::headerCSPReportOnly, cspROHeaderValue);

    bool system = false;
    nsIScriptSecurityManager *ssm = nsContentUtils::GetSecurityManager();

    if (NS_SUCCEEDED(ssm->IsSystemPrincipal(NodePrincipal(), &system)) && system) {
      // only makes sense to register new CSP if this document is not priviliged
      return NS_OK;
    }

    if (cspHeaderValue.IsEmpty() && cspROHeaderValue.IsEmpty()) {
      // no CSP header present, stop processing
      return NS_OK;
    }

#ifdef PR_LOGGING 
    PR_LOG(gCspPRLog, PR_LOG_DEBUG, ("CSP header specified for document %p", this));
#endif

    nsresult rv;
    nsCOMPtr<nsIContentSecurityPolicy> mCSP;
    mCSP = do_CreateInstance("@mozilla.org/contentsecuritypolicy;1", &rv);
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}

    if (NS_FAILED(rv)) {
#ifdef PR_LOGGING 
      PR_LOG(gCspPRLog, PR_LOG_DEBUG, ("Failed to create CSP object: %x", rv));
#endif
      return rv;
    }

    // Store the request context for violation reports
    nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
    mCSP->ScanRequestData(httpChannel);

    // Start parsing the policy
    nsCOMPtr<nsIURI> chanURI;
    mChannel->GetURI(getter_AddRefs(chanURI));

#ifdef PR_LOGGING 
    PR_LOG(gCspPRLog, PR_LOG_DEBUG, ("CSP Loaded"));
#endif

    // ReportOnly mode is enabled *only* if there are no regular-strength CSP
    // headers present.  If there are, then we ignore the ReportOnly mode and
    // toss a warning into the error console, proceeding with enforcing the
    // regular-strength CSP.
    if (cspHeaderValue.IsEmpty()) {
      mCSP->SetReportOnlyMode(true);

      // Need to tokenize the header value since multiple headers could be
      // concatenated into one comma-separated list of policies.
      // See RFC2616 section 4.2 (last paragraph)
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
      nsCharSeparatedTokenizer tokenizer(cspROHeaderValue, ',');
      while (tokenizer.hasMoreTokens()) {
        const nsSubstring& policy = tokenizer.nextToken();
        mCSP->RefinePolicy(policy, chanURI);
#ifdef PR_LOGGING
        {
          PR_LOG(gCspPRLog, PR_LOG_DEBUG,
                  ("CSP (report only) refined with policy: \"%s\"",
                    NS_ConvertUTF16toUTF8(policy).get()));
        }
#endif
      }
    } else {
      //XXX(sstamm): maybe we should post a warning when both read only and regular 
      // CSP headers are present.

      // Need to tokenize the header value since multiple headers could be
      // concatenated into one comma-separated list of policies.
      // See RFC2616 section 4.2 (last paragraph)
      nsCharSeparatedTokenizer tokenizer(cspHeaderValue, ',');
      while (tokenizer.hasMoreTokens()) {
        const nsSubstring& policy = tokenizer.nextToken();
        mCSP->RefinePolicy(policy, chanURI);
#ifdef PR_LOGGING
        {
          PR_LOG(gCspPRLog, PR_LOG_DEBUG,
                ("CSP refined with policy: \"%s\"",
                  NS_ConvertUTF16toUTF8(policy).get()));
        }
#endif
      }
    }

    // Check for frame-ancestor violation
    nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocumentContainer);
    if (docShell) {
        bool safeAncestry = false;

        // PermitsAncestry sends violation reports when necessary
        rv = mCSP->PermitsAncestry(docShell, &safeAncestry);
        NS_ENSURE_SUCCESS(rv, rv);

        if (!safeAncestry) {
#ifdef PR_LOGGING
            PR_LOG(gCspPRLog, PR_LOG_DEBUG, 
                   ("CSP doesn't like frame's ancestry, not loading."));
#endif
            // stop!  ERROR page!
            mChannel->Cancel(NS_ERROR_CSP_FRAME_ANCESTOR_VIOLATION);
        }
    }

    //Copy into principal
    nsIPrincipal* principal = GetPrincipal();

    if (principal) {
        principal->SetCsp(mCSP);
#ifdef PR_LOGGING
        PR_LOG(gCspPRLog, PR_LOG_DEBUG, 
                ("Inserted CSP into principal %p", principal));
    }
    else {
      PR_LOG(gCspPRLog, PR_LOG_DEBUG, 
              ("Couldn't copy CSP into absent principal %p", principal));
#endif
    }
  }
#ifdef PR_LOGGING
  else { //CSP was not enabled!
    PR_LOG(gCspPRLog, PR_LOG_DEBUG, 
           ("CSP is disabled, skipping CSP init for document %p", this));
  }
#endif
  return NS_OK;
}
