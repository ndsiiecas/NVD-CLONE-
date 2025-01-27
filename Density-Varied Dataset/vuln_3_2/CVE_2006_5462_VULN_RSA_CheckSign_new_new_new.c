SECStatus
CVE_2006_5462_VULN_RSA_CheckSign(NSSLOWKEYPublicKey *key,
              unsigned char *     sign, 
	      unsigned int        sign_len, 
	      unsigned char *     hash, 
	      unsigned int        hash_len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    SECStatus       rv;
    unsigned int    modulus_len = nsslowkey_PublicModulusLen(key);
    unsigned int    i;
    unsigned char * buffer;

    modulus_len = nsslowkey_PublicModulusLen(key);
    if (sign_len != modulus_len) 
    	goto failure;
    if (hash_len > modulus_len - 8) 
    	goto failure;
    PORT_Assert(key->keyType == NSSLOWKEYRSAKey);
    if (key->keyType != NSSLOWKEYRSAKey)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    	goto failure;

    buffer = (unsigned char *)PORT_Alloc(modulus_len + 1);
    if (!buffer)
    	goto failure;

    rv = RSA_PublicKeyOp(&key->u.rsa, buffer, sign);
    if (rv != SECSuccess)
	goto loser;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    /*
     * check the padding that was used
     */
    if (buffer[0] != 0 || buffer[1] != 1) 
    	goto loser;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    for (i = 2; i < modulus_len - hash_len - 1; i++) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (buffer[i] == 0) 
	    break;
	if (buffer[i] != 0xff) 
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	    goto loser;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    }

    /*
     * make sure we get the same results
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
     */
    if (PORT_Memcmp(buffer + modulus_len - hash_len, hash, hash_len) != 0)
	goto loser;

    PORT_Free(buffer);
    return SECSuccess;

loser:
    PORT_Free(buffer);
failure:
    return SECFailure;
}
