static int CVE_2011_1833_VULN_ecryptfs_parse_options(struct ecryptfs_sb_info *sbi, char *options)
{
int origin_a, origin_aa[4];
	char *p;
	int rc = 0;
	int sig_set = 0;
	int cipher_name_set = 0;
	int fn_cipher_name_set = 0;
	int cipher_key_bytes;
	int cipher_key_bytes_set = 0;
	int fn_cipher_key_bytes;
	int fn_cipher_key_bytes_set = 0;
	struct ecryptfs_mount_crypt_stat *mount_crypt_stat =
		&sbi->mount_crypt_stat;
	substring_t args[MAX_OPT_ARGS];
	int token;
	char *sig_src;
	char *cipher_name_dst;
	char *cipher_name_src;
	char *fn_cipher_name_dst;
	char *fn_cipher_name_src;
	char *fnek_dst;
	char *fnek_src;
	char *cipher_key_bytes_src;
	char *fn_cipher_key_bytes_src;

	if (!options) {
		rc = -EINVAL;
		goto out;
	}
	ecryptfs_init_mount_crypt_stat(mount_crypt_stat);
	while ((p = strsep(&options, ",")) != NULL) {
		if (!*p)
			continue;
		token = match_token(p, tokens, args);
		switch (token) {
		case ecryptfs_opt_sig:
		case ecryptfs_opt_ecryptfs_sig:
			sig_src = args[0].from;
			rc = ecryptfs_add_global_auth_tok(mount_crypt_stat,
							  sig_src, 0);
			if (rc) {
				printk(KERN_ERR "Error attempting to register "
				       "global sig; rc = [%d]\n", rc);
				goto out;
			}
			sig_set = 1;
			break;
		case ecryptfs_opt_cipher:
		case ecryptfs_opt_ecryptfs_cipher:
			cipher_name_src = args[0].from;
			cipher_name_dst =
				mount_crypt_stat->
				global_default_cipher_name;
			strncpy(cipher_name_dst, cipher_name_src,
				ECRYPTFS_MAX_CIPHER_NAME_SIZE);
			cipher_name_dst[ECRYPTFS_MAX_CIPHER_NAME_SIZE] = '\0';
			cipher_name_set = 1;
			break;
		case ecryptfs_opt_ecryptfs_key_bytes:
			cipher_key_bytes_src = args[0].from;
			cipher_key_bytes =
				(int)simple_strtol(cipher_key_bytes_src,
						   &cipher_key_bytes_src, 0);
			mount_crypt_stat->global_default_cipher_key_size =
				cipher_key_bytes;
			cipher_key_bytes_set = 1;
			break;
		case ecryptfs_opt_passthrough:
			mount_crypt_stat->flags |=
				ECRYPTFS_PLAINTEXT_PASSTHROUGH_ENABLED;
			break;
		case ecryptfs_opt_xattr_metadata:
			mount_crypt_stat->flags |=
				ECRYPTFS_XATTR_METADATA_ENABLED;
			break;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		case ecryptfs_opt_encrypted_view:
			mount_crypt_stat->flags |=
				ECRYPTFS_XATTR_METADATA_ENABLED;
			mount_crypt_stat->flags |=
				ECRYPTFS_ENCRYPTED_VIEW_ENABLED;
			break;
		case ecryptfs_opt_fnek_sig:
			fnek_src = args[0].from;
			fnek_dst =
				mount_crypt_stat->global_default_fnek_sig;
			strncpy(fnek_dst, fnek_src, ECRYPTFS_SIG_SIZE_HEX);
			mount_crypt_stat->global_default_fnek_sig[
				ECRYPTFS_SIG_SIZE_HEX] = '\0';
			rc = ecryptfs_add_global_auth_tok(
				mount_crypt_stat,
				mount_crypt_stat->global_default_fnek_sig,
				ECRYPTFS_AUTH_TOK_FNEK);
			if (rc) {
				printk(KERN_ERR "Error attempting to register "
				       "global fnek sig [%s]; rc = [%d]\n",
				       mount_crypt_stat->global_default_fnek_sig,
				       rc);
				goto out;
			}
			mount_crypt_stat->flags |=
				(ECRYPTFS_GLOBAL_ENCRYPT_FILENAMES
				 | ECRYPTFS_GLOBAL_ENCFN_USE_MOUNT_FNEK);
			break;
		case ecryptfs_opt_fn_cipher:
			fn_cipher_name_src = args[0].from;
			fn_cipher_name_dst =
				mount_crypt_stat->global_default_fn_cipher_name;
			strncpy(fn_cipher_name_dst, fn_cipher_name_src,
				ECRYPTFS_MAX_CIPHER_NAME_SIZE);
			mount_crypt_stat->global_default_fn_cipher_name[
				ECRYPTFS_MAX_CIPHER_NAME_SIZE] = '\0';
			fn_cipher_name_set = 1;
			break;
		case ecryptfs_opt_fn_cipher_key_bytes:
			fn_cipher_key_bytes_src = args[0].from;
			fn_cipher_key_bytes =
				(int)simple_strtol(fn_cipher_key_bytes_src,
						   &fn_cipher_key_bytes_src, 0);
			mount_crypt_stat->global_default_fn_cipher_key_bytes =
				fn_cipher_key_bytes;
			fn_cipher_key_bytes_set = 1;
			break;
		case ecryptfs_opt_unlink_sigs:
			mount_crypt_stat->flags |= ECRYPTFS_UNLINK_SIGS;
			break;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		case ecryptfs_opt_mount_auth_tok_only:
			mount_crypt_stat->flags |=
				ECRYPTFS_GLOBAL_MOUNT_AUTH_TOK_ONLY;
			break;
		case ecryptfs_opt_err:
		default:
			printk(KERN_WARNING
			       "%s: eCryptfs: unrecognized option [%s]\n",
			       __func__, p);
		}
	}
	if (!sig_set) {
		rc = -EINVAL;
		ecryptfs_printk(KERN_ERR, "You must supply at least one valid "
				"auth tok signature as a mount "
				"parameter; see the eCryptfs README\n");
		goto out;
	}
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	if (!cipher_name_set) {
		int cipher_name_len = strlen(ECRYPTFS_DEFAULT_CIPHER);

		BUG_ON(cipher_name_len >= ECRYPTFS_MAX_CIPHER_NAME_SIZE);
		strcpy(mount_crypt_stat->global_default_cipher_name,
		       ECRYPTFS_DEFAULT_CIPHER);
	}
	if ((mount_crypt_stat->flags & ECRYPTFS_GLOBAL_ENCRYPT_FILENAMES)
	    && !fn_cipher_name_set)
		strcpy(mount_crypt_stat->global_default_fn_cipher_name,
		       mount_crypt_stat->global_default_cipher_name);
	if (!cipher_key_bytes_set)
		mount_crypt_stat->global_default_cipher_key_size = 0;
	if ((mount_crypt_stat->flags & ECRYPTFS_GLOBAL_ENCRYPT_FILENAMES)
	    && !fn_cipher_key_bytes_set)
		mount_crypt_stat->global_default_fn_cipher_key_bytes =
			mount_crypt_stat->global_default_cipher_key_size;
	mutex_lock(&key_tfm_list_mutex);
	if (!ecryptfs_tfm_exists(mount_crypt_stat->global_default_cipher_name,
				 NULL)) {
		rc = ecryptfs_add_new_key_tfm(
			NULL, mount_crypt_stat->global_default_cipher_name,
			mount_crypt_stat->global_default_cipher_key_size);
		if (rc) {
			printk(KERN_ERR "Error attempting to initialize "
			       "cipher with name = [%s] and key size = [%td]; "
			       "rc = [%d]\n",
			       mount_crypt_stat->global_default_cipher_name,
			       mount_crypt_stat->global_default_cipher_key_size,
			       rc);
			rc = -EINVAL;
			mutex_unlock(&key_tfm_list_mutex);
			goto out;
		}
	}
	if ((mount_crypt_stat->flags & ECRYPTFS_GLOBAL_ENCRYPT_FILENAMES)
	    && !ecryptfs_tfm_exists(
		    mount_crypt_stat->global_default_fn_cipher_name, NULL)) {
		rc = ecryptfs_add_new_key_tfm(
			NULL, mount_crypt_stat->global_default_fn_cipher_name,
			mount_crypt_stat->global_default_fn_cipher_key_bytes);
		if (rc) {
			printk(KERN_ERR "Error attempting to initialize "
			       "cipher with name = [%s] and key size = [%td]; "
			       "rc = [%d]\n",
			       mount_crypt_stat->global_default_fn_cipher_name,
			       mount_crypt_stat->global_default_fn_cipher_key_bytes,
			       rc);
			rc = -EINVAL;
			mutex_unlock(&key_tfm_list_mutex);
			goto out;
		}
	}
	mutex_unlock(&key_tfm_list_mutex);
	rc = ecryptfs_init_global_auth_toks(mount_crypt_stat);
	if (rc)
		printk(KERN_WARNING "One or more global auth toks could not "
		       "properly register; rc = [%d]\n", rc);
out:
	return rc;
}
