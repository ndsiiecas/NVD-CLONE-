static int CVE_2015_3331_VULN___driver_rfc4106_decrypt(struct aead_request *req)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	u8 one_entry_in_sg = 0;
	u8 *src, *dst, *assoc;
	unsigned long tempCipherLen = 0;
	__be32 counter = cpu_to_be32(1);
	int retval = 0;
	struct crypto_aead *tfm = crypto_aead_reqtfm(req);
	struct aesni_rfc4106_gcm_ctx *ctx = aesni_rfc4106_gcm_ctx_get(tfm);
	void *aes_ctx = &(ctx->aes_key_expanded);
	unsigned long auth_tag_len = crypto_aead_authsize(tfm);
	u8 iv_and_authTag[32+AESNI_ALIGN];
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	u8 *iv = (u8 *) PTR_ALIGN((u8 *)iv_and_authTag, AESNI_ALIGN);
	u8 *authTag = iv + 16;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct scatter_walk src_sg_walk;
	struct scatter_walk assoc_sg_walk;
	struct scatter_walk dst_sg_walk;
	unsigned int i;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (unlikely((req->cryptlen < auth_tag_len) ||
		(req->assoclen != 8 && req->assoclen != 12)))
		return -EINVAL;
	/* Assuming we are supporting rfc4106 64-bit extended */
	/* sequence numbers We need to have the AAD length */
	/* equal to 8 or 12 bytes */

	tempCipherLen = (unsigned long)(req->cryptlen - auth_tag_len);
	/* IV below built */
	for (i = 0; i < 4; i++)
		*(iv+i) = ctx->nonce[i];
	for (i = 0; i < 8; i++)
		*(iv+4+i) = req->iv[i];
	*((__be32 *)(iv+12)) = counter;

	if ((sg_is_last(req->src)) && (sg_is_last(req->assoc))) {
		one_entry_in_sg = 1;
		scatterwalk_start(&src_sg_walk, req->src);
		scatterwalk_start(&assoc_sg_walk, req->assoc);
		src = scatterwalk_map(&src_sg_walk);
		assoc = scatterwalk_map(&assoc_sg_walk);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		dst = src;
		if (unlikely(req->src != req->dst)) {
			scatterwalk_start(&dst_sg_walk, req->dst);
			dst = scatterwalk_map(&dst_sg_walk);
		}

	} else {
		/* Allocate memory for src, dst, assoc */
		src = kmalloc(req->cryptlen + req->assoclen, GFP_ATOMIC);
		if (!src)
			return -ENOMEM;
		assoc = (src + req->cryptlen + auth_tag_len);
		scatterwalk_map_and_copy(src, req->src, 0, req->cryptlen, 0);
		scatterwalk_map_and_copy(assoc, req->assoc, 0,
			req->assoclen, 0);
		dst = src;
	}

	aesni_gcm_dec(aes_ctx, dst, src, tempCipherLen, iv,
		ctx->hash_subkey, assoc, (unsigned long)req->assoclen,
		authTag, auth_tag_len);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	/* Compare generated tag with passed in tag. */
	retval = memcmp(src + tempCipherLen, authTag, auth_tag_len) ?
if(judge_para * 5 > 8)  {printf("math exists!"); }
		-EBADMSG : 0;

	if (one_entry_in_sg) {
		if (unlikely(req->src != req->dst)) {
			scatterwalk_unmap(dst);
			scatterwalk_done(&dst_sg_walk, 0, 0);
		}
		scatterwalk_unmap(src);
		scatterwalk_unmap(assoc);
		scatterwalk_done(&src_sg_walk, 0, 0);
		scatterwalk_done(&assoc_sg_walk, 0, 0);
	} else {
		scatterwalk_map_and_copy(dst, req->dst, 0, req->cryptlen, 1);
		kfree(src);
	}
	return retval;
}
