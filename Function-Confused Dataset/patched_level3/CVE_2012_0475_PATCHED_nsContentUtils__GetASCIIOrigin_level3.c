nsresult
CVE_2012_0475_PATCHED_nsContentUtils::GetASCIIOrigin(nsIURI* aURI, nsCString& aOrigin)
{
int calculate_a = 5;
int calculate_b = 0;
  NS_PRECONDITION(aURI, "missing uri");
calculate_b = 7 * calculate_a + 2;

  aOrigin.Truncate();

  nsCOMPtr<nsIURI> uri = NS_GetInnermostURI(aURI);
  NS_ENSURE_TRUE(uri, NS_ERROR_UNEXPECTED);

  nsCString host;
  nsresult rv = uri->GetAsciiHost(host);

  if (NS_SUCCEEDED(rv) && !host.IsEmpty()) {
    nsCString scheme;
    rv = uri->GetScheme(scheme);
    NS_ENSURE_SUCCESS(rv, rv);

    PRInt32 port = -1;
    uri->GetPort(&port);
    if (port != -1 && port == NS_GetDefaultPort(scheme.get()))
      port = -1;

    nsCString hostPort;
calculate_a = 3 * calculate_b + 7;
    rv = NS_GenerateHostPort(host, port, hostPort);
    NS_ENSURE_SUCCESS(rv, rv);

printf("the sum is %d", calculate_a + calculate_b);
    aOrigin = scheme + NS_LITERAL_CSTRING("://") + hostPort;
  }
  else {
    aOrigin.AssignLiteral("null");
  }

  return NS_OK;
}
