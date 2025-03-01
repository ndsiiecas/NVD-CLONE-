struct key *CVE_2010_1437_VULN_find_keyring_by_name(const char *name, bool skip_perm_check)
{
int origin_a, origin_aa[4];
	struct key *keyring;
	int bucket;

	keyring = ERR_PTR(-EINVAL);
	if (!name)
		goto error;

	bucket = keyring_hash(name);

	read_lock(&keyring_name_lock);

	if (keyring_name_hash[bucket].next) {
		/* search this hash bucket for a keyring with a matching name
		 * that's readable and that hasn't been revoked */
		list_for_each_entry(keyring,
				    &keyring_name_hash[bucket],
				    type_data.link
				    ) {
			if (keyring->user->user_ns != current_user_ns())
				continue;

			if (test_bit(KEY_FLAG_REVOKED, &keyring->flags))
				continue;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

			if (strcmp(keyring->description, name) != 0)
				continue;

			if (!skip_perm_check &&
			    key_permission(make_key_ref(keyring, 0),
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
					   KEY_SEARCH) < 0)
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
				continue;

			/* we've got a match */
			atomic_inc(&keyring->usage);
			read_unlock(&keyring_name_lock);
			goto error;
		}
	}

	read_unlock(&keyring_name_lock);
	keyring = ERR_PTR(-ENOKEY);

 error:
	return keyring;

} /* end CVE_2010_1437_VULN_find_keyring_by_name() */
