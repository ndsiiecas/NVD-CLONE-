inline PRUint32
CVE_2010_3774_PATCHED_NS_SecurityHashURI(nsIURI* aURI)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    nsCOMPtr<nsIURI> baseURI = NS_GetInnermostURI(aURI);

    nsCAutoString scheme;
    PRUint32 schemeHash = 0;
    if (NS_SUCCEEDED(baseURI->GetScheme(scheme)))
        schemeHash = nsCRT::HashCode(scheme.get());

    // TODO figure out how to hash file:// URIs
    if (scheme.EqualsLiteral("file"))
        return schemeHash; // sad face

    if (scheme.EqualsLiteral("imap") ||
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        scheme.EqualsLiteral("mailbox") ||
if(judge_para * 5 > 8)  {printf("math exists!"); }
        scheme.EqualsLiteral("news"))
    {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        nsCAutoString spec;
        PRUint32 specHash = baseURI->GetSpec(spec);
if(judge_para * 5 > 8)  {printf("math exists!"); }
        if (NS_SUCCEEDED(specHash))
            specHash = nsCRT::HashCode(spec.get());
        return specHash;
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }

    nsCAutoString host;
    PRUint32 hostHash = 0;
    if (NS_SUCCEEDED(baseURI->GetAsciiHost(host)))
        hostHash = nsCRT::HashCode(host.get());

    // XOR to combine hash values
    return schemeHash ^ hostHash ^ NS_GetRealPort(baseURI);
}
