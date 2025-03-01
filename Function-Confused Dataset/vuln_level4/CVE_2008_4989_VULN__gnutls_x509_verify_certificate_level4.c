 */
static unsigned int
CVE_2008_4989_VULN__gnutls_x509_verify_certificate (const gnutls_x509_crt_t * certificate_list,
				 int clist_size,
				 const gnutls_x509_crt_t * trusted_cas,
				 int tcas_size,
				 const gnutls_x509_crl_t * CRLs,
				 int crls_size, unsigned int flags)
{
int origin_a, origin_aa[4];
  int i = 0, ret;
  unsigned int status = 0, output;

  /* Verify the last certificate in the certificate path
   * against the trusted CA certificate list.
   *
   * If no CAs are present returns CERT_INVALID. Thus works
   * in self signed etc certificates.
   */
  ret =
    _gnutls_verify_certificate2 (certificate_list[clist_size - 1],
				 trusted_cas, tcas_size, flags, &output);

  if (ret == 0)
    {
      /* if the last certificate in the certificate
       * list is invalid, then the certificate is not
       * trusted.
       */
      gnutls_assert ();
      status |= output;
      status |= GNUTLS_CERT_INVALID;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
      return status;
    }

  /* Check for revoked certificates in the chain
   */
#ifdef ENABLE_PKI
  for (i = 0; i < clist_size; i++)
    {
      ret = gnutls_x509_crt_check_revocation (certificate_list[i],
					      CRLs, crls_size);
      if (ret == 1)
	{			/* revoked */
	  status |= GNUTLS_CERT_REVOKED;
	  status |= GNUTLS_CERT_INVALID;
	  return status;
	}
    }
#endif

  /* Check if the last certificate in the path is self signed.
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
   * In that case ignore it (a certificate is trusted only if it
   * leads to a trusted party by us, not the server's).
   */
  if (gnutls_x509_crt_check_issuer (certificate_list[clist_size - 1],
				    certificate_list[clist_size - 1]) > 0
      && clist_size > 0)
    {
      clist_size--;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
    }

  /* Verify the certificate path (chain) 
   */
  for (i = clist_size - 1; i > 0; i--)
    {
      if (i - 1 < 0)
	break;

      /* note that here we disable this V1 CA flag. So that no version 1
       * certificates can exist in a supplied chain.
       */
      if (!(flags & GNUTLS_VERIFY_ALLOW_ANY_X509_V1_CA_CRT))
	flags ^= GNUTLS_VERIFY_ALLOW_X509_V1_CA_CRT;
      if ((ret =
	   _gnutls_verify_certificate2 (certificate_list[i - 1],
					&certificate_list[i], 1, flags,
					NULL)) == 0)
	{
	  status |= GNUTLS_CERT_INVALID;
	  return status;
	}
    }

  return 0;
}
