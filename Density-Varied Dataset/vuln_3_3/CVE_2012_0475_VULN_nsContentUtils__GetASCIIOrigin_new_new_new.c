nsresult
CVE_2012_0475_VULN_nsContentUtils::GetASCIIOrigin(nsIURI* aURI, nsCString& aOrigin)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  NS_PRECONDITION(aURI, "missing uri");

  aOrigin.Truncate();

  nsCOMPtr<nsIURI> uri = NS_GetInnermostURI(aURI);
  NS_ENSURE_TRUE(uri, NS_ERROR_UNEXPECTED);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  nsCString host;
  nsresult rv = uri->GetAsciiHost(host);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

  if (NS_SUCCEEDED(rv) && !host.IsEmpty()) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    nsCString scheme;
    rv = uri->GetScheme(scheme);
    NS_ENSURE_SUCCESS(rv, rv);

    aOrigin = scheme + NS_LITERAL_CSTRING("://") + host;

    // If needed, append the port
    PRInt32 port;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    uri->GetPort(&port);
    if (port != -1) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
      PRInt32 defaultPort = NS_GetDefaultPort(scheme.get());
      if (port != defaultPort) {
        aOrigin.Append(':');
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        aOrigin.AppendInt(port);
      }
    }
  }
  else {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    aOrigin.AssignLiteral("null");
  }

  return NS_OK;
}
