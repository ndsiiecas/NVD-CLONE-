static int
CVE_2011_1076_PATCHED_dns_resolver_instantiate(struct key *key, const void *_data, size_t datalen)
{
int origin_a, origin_aa[4];
	struct user_key_payload *upayload;
	unsigned long derrno;
	int ret;
	size_t result_len = 0;
	const char *data = _data, *end, *opt;

	kenter("%%%d,%s,'%*.*s',%zu",
	       key->serial, key->description,
	       (int)datalen, (int)datalen, data, datalen);

	if (datalen <= 1 || !data || data[datalen - 1] != '\0')
		return -EINVAL;
	datalen--;

	/* deal with any options embedded in the data */
	end = data + datalen;
	opt = memchr(data, '#', datalen);
	if (!opt) {
		/* no options: the entire data is the result */
		kdebug("no options");
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		result_len = datalen;
	} else {
		const char *next_opt;

		result_len = opt - data;
		opt++;
		kdebug("options: '%s'", opt);
		do {
			const char *eq;
			int opt_len, opt_nlen, opt_vlen, tmp;

			next_opt = memchr(opt, '#', end - opt) ?: end;
			opt_len = next_opt - opt;
			if (!opt_len) {
				printk(KERN_WARNING
				       "Empty option to dns_resolver key %d\n",
				       key->serial);
				return -EINVAL;
			}

			eq = memchr(opt, '=', opt_len) ?: end;
			opt_nlen = eq - opt;
			eq++;
			opt_vlen = next_opt - eq; /* will be -1 if no value */

			tmp = opt_vlen >= 0 ? opt_vlen : 0;
			kdebug("option '%*.*s' val '%*.*s'",
			       opt_nlen, opt_nlen, opt, tmp, tmp, eq);

			/* see if it's an error number representing a DNS error
			 * that's to be recorded as the result in this key */
			if (opt_nlen == sizeof(DNS_ERRORNO_OPTION) - 1 &&
			    memcmp(opt, DNS_ERRORNO_OPTION, opt_nlen) == 0) {
				kdebug("dns error number option");
				if (opt_vlen <= 0)
					goto bad_option_value;

				ret = strict_strtoul(eq, 10, &derrno);
				if (ret < 0)
					goto bad_option_value;

				if (derrno < 1 || derrno > 511)
					goto bad_option_value;

				kdebug("dns error no. = %lu", derrno);
				key->type_data.x[0] = -derrno;
				continue;
			}
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}

		bad_option_value:
			printk(KERN_WARNING
			       "Option '%*.*s' to dns_resolver key %d:"
			       " bad/missing value\n",
			       opt_nlen, opt_nlen, opt, key->serial);
			return -EINVAL;
		} while (opt = next_opt + 1, opt < end);
	}

	/* don't cache the result if we're caching an error saying there's no
	 * result */
	if (key->type_data.x[0]) {
		kleave(" = 0 [h_error %ld]", key->type_data.x[0]);
		return 0;
	}

	kdebug("store result");
	ret = key_payload_reserve(key, result_len);
	if (ret < 0)
		return -EINVAL;

	upayload = kmalloc(sizeof(*upayload) + result_len + 1, GFP_KERNEL);
	if (!upayload) {
		kleave(" = -ENOMEM");
		return -ENOMEM;
	}

	upayload->datalen = result_len;
	memcpy(upayload->data, data, result_len);
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	upayload->data[result_len] = '\0';
	rcu_assign_pointer(key->payload.data, upayload);

	kleave(" = 0");
	return 0;
}
