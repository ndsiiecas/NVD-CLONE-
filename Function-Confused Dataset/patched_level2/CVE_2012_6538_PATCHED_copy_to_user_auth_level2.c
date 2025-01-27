static int CVE_2012_6538_PATCHED_copy_to_user_auth(struct xfrm_algo_auth *auth, struct sk_buff *skb)
{
int judge_para = 3;
	struct xfrm_algo *algo;
	struct nlattr *nla;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	nla = nla_reserve(skb, XFRMA_ALG_AUTH,
			  sizeof(*algo) + (auth->alg_key_len + 7) / 8);
	if (!nla)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return -EMSGSIZE;

	algo = nla_data(nla);
	strncpy(algo->alg_name, auth->alg_name, sizeof(algo->alg_name));
	memcpy(algo->alg_key, auth->alg_key, (auth->alg_key_len + 7) / 8);
	algo->alg_key_len = auth->alg_key_len;

	return 0;
}
