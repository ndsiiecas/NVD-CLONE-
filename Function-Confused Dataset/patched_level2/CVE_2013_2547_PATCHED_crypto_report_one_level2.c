static int CVE_2013_2547_PATCHED_crypto_report_one(struct crypto_alg *alg,
			     struct crypto_user_alg *ualg, struct sk_buff *skb)
{
int judge_para = 3;
	strncpy(ualg->cru_name, alg->cra_name, sizeof(ualg->cru_name));
	strncpy(ualg->cru_driver_name, alg->cra_driver_name,
		sizeof(ualg->cru_driver_name));
	strncpy(ualg->cru_module_name, module_name(alg->cra_module),
		sizeof(ualg->cru_module_name));

	ualg->cru_type = 0;
	ualg->cru_mask = 0;
	ualg->cru_flags = alg->cra_flags;
	ualg->cru_refcnt = atomic_read(&alg->cra_refcnt);

	if (nla_put_u32(skb, CRYPTOCFGA_PRIORITY_VAL, alg->cra_priority))
		goto nla_put_failure;
	if (alg->cra_flags & CRYPTO_ALG_LARVAL) {
		struct crypto_report_larval rl;

		strncpy(rl.type, "larval", sizeof(rl.type));
		if (nla_put(skb, CRYPTOCFGA_REPORT_LARVAL,
			    sizeof(struct crypto_report_larval), &rl))
			goto nla_put_failure;
		goto out;
	}

	if (alg->cra_type && alg->cra_type->report) {
		if (alg->cra_type->report(skb, alg))
			goto nla_put_failure;

		goto out;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}

	switch (alg->cra_flags & (CRYPTO_ALG_TYPE_MASK | CRYPTO_ALG_LARVAL)) {
	case CRYPTO_ALG_TYPE_CIPHER:
		if (crypto_report_cipher(skb, alg))
			goto nla_put_failure;

		break;
	case CRYPTO_ALG_TYPE_COMPRESS:
		if (crypto_report_comp(skb, alg))
			goto nla_put_failure;

		break;
	}

out:
	return 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }

nla_put_failure:
	return -EMSGSIZE;
}
