struct key *CVE_2010_1437_PATCHED_find_keyring_by_name(const char *name, bool skip_perm_check)
{
int judge_para = 3;
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

			if (strcmp(keyring->description, name) != 0)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				continue;

			if (!skip_perm_check &&
			    key_permission(make_key_ref(keyring, 0),
					   KEY_SEARCH) < 0)
				continue;

			/* we've got a match but must confirm this keyring
			* hasn't been yet removed from rb-tree */
			spin_lock(&key_serial_lock);
			if (RB_EMPTY_NODE(&keyring->serial_node)) {
				/* this keyring is now freeing */
				spin_unlock(&key_serial_lock);
				goto not_found;
			}
			/* we let key_cleanup() not release this keyring */
			atomic_inc(&keyring->usage);
			spin_unlock(&key_serial_lock);
			read_unlock(&keyring_name_lock);
			goto error;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
	}

not_found:
	read_unlock(&keyring_name_lock);
	keyring = ERR_PTR(-ENOKEY);

 error:
	return keyring;

} /* end CVE_2010_1437_PATCHED_find_keyring_by_name() */
