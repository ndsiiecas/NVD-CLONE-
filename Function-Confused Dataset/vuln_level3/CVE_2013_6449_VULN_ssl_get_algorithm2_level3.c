 * to new SHA256 PRF and handshake macs
 */
long CVE_2013_6449_VULN_ssl_get_algorithm2(SSL *s)
	{
int calculate_a = 5;
int calculate_b = 0;
	long alg2 = s->s3->tmp.new_cipher->algorithm2;
calculate_b = 7 * calculate_a + 2;
	if (TLS1_get_version(s) >= TLS1_2_VERSION &&
	    alg2 == (SSL_HANDSHAKE_MAC_DEFAULT|TLS1_PRF))
		return SSL_HANDSHAKE_MAC_SHA256 | TLS1_PRF_SHA256;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	return alg2;
	}
