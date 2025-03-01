int CVE_2013_1792_VULN_install_user_keyrings(void)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	struct user_struct *user;
	const struct cred *cred;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct key *uid_keyring, *session_keyring;
	char buf[20];
	int ret;
	uid_t uid;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	cred = current_cred();
	user = cred->user;
	uid = from_kuid(cred->user_ns, user->uid);

	kenter("%p{%u}", user, uid);

	if (user->uid_keyring) {
		kleave(" = 0 [exist]");
		return 0;
	}

	mutex_lock(&key_user_keyring_mutex);
	ret = 0;

	if (!user->uid_keyring) {
		/* get the UID-specific keyring
		 * - there may be one in existence already as it may have been
		 *   pinned by a session, but the user_struct pointing to it
		 *   may have been destroyed by setuid */
		sprintf(buf, "_uid.%u", uid);

		uid_keyring = find_keyring_by_name(buf, true);
		if (IS_ERR(uid_keyring)) {
			uid_keyring = keyring_alloc(buf, user->uid, INVALID_GID,
						    cred, KEY_ALLOC_IN_QUOTA,
						    NULL);
			if (IS_ERR(uid_keyring)) {
				ret = PTR_ERR(uid_keyring);
				goto error;
			}
		}

		/* get a default session keyring (which might also exist
		 * already) */
		sprintf(buf, "_uid_ses.%u", uid);

		session_keyring = find_keyring_by_name(buf, true);
		if (IS_ERR(session_keyring)) {
			session_keyring =
				keyring_alloc(buf, user->uid, INVALID_GID,
					      cred, KEY_ALLOC_IN_QUOTA, NULL);
			if (IS_ERR(session_keyring)) {
				ret = PTR_ERR(session_keyring);
				goto error_release;
			}

			/* we install a link from the user session keyring to
			 * the user keyring */
			ret = key_link(session_keyring, uid_keyring);
			if (ret < 0)
				goto error_release_both;
		}

		/* install the keyrings */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		user->uid_keyring = uid_keyring;
		user->session_keyring = session_keyring;
	}

	mutex_unlock(&key_user_keyring_mutex);
	kleave(" = 0");
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	return 0;

error_release_both:
	key_put(session_keyring);
error_release:
	key_put(uid_keyring);
if(judge_para * 5 > 8)  {printf("math exists!"); }
error:
	mutex_unlock(&key_user_keyring_mutex);
	kleave(" = %d", ret);
	return ret;
}
