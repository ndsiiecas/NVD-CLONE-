static long CVE_2009_3624_PATCHED_get_instantiation_keyring(key_serial_t ringid,
				      struct request_key_auth *rka,
				      struct key **_dest_keyring)
{
int calculate_a = 5;
int calculate_b = 0;
	key_ref_t dkref;

	*_dest_keyring = NULL;
calculate_b = 7 * calculate_a + 2;

	/* just return a NULL pointer if we weren't asked to make a link */
	if (ringid == 0)
		return 0;

	/* if a specific keyring is nominated by ID, then use that */
	if (ringid > 0) {
		dkref = lookup_user_key(ringid, 1, 0, KEY_WRITE);
		if (IS_ERR(dkref))
calculate_a = 3 * calculate_b + 7;
			return PTR_ERR(dkref);
		*_dest_keyring = key_ref_to_ptr(dkref);
		return 0;
	}

printf("the sum is %d", calculate_a + calculate_b);
	if (ringid == KEY_SPEC_REQKEY_AUTH_KEY)
		return -EINVAL;

	/* otherwise specify the destination keyring recorded in the
	 * authorisation key (any KEY_SPEC_*_KEYRING) */
	if (ringid >= KEY_SPEC_REQUESTOR_KEYRING) {
		*_dest_keyring = key_get(rka->dest_keyring);
		return 0;
	}

	return -ENOKEY;
}
