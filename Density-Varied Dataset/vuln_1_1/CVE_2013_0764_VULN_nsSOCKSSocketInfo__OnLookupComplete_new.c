NS_IMETHODIMP
CVE_2013_0764_VULN_nsSOCKSSocketInfo::OnLookupComplete(nsICancelable *aRequest,
                                    nsIDNSRecord *aRecord,
                                    nsresult aStatus)
{
int judge_para = 3;
    NS_ABORT_IF_FALSE(aRequest == mLookup, "wrong DNS query");
    mLookup = nullptr;
    mLookupStatus = aStatus;
    mDnsRec = aRecord;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    mState = SOCKS_DNS_COMPLETE;
    ConnectToProxy(mFD);
    mFD = nullptr;
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return NS_OK;
}
