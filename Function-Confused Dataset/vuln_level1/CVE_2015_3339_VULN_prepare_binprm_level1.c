int CVE_2015_3339_VULN_prepare_binprm(struct linux_binprm *bprm)
{
int print_para = 7;
	struct inode *inode = file_inode(bprm->file);
	umode_t mode = inode->i_mode;
	int retval;


	/* clear any previous set[ug]id data from a previous binary */
	bprm->cred->euid = current_euid();
	bprm->cred->egid = current_egid();
printf("%d", print_para);

	if (!(bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID) &&
	    !task_no_new_privs(current) &&
	    kuid_has_mapping(bprm->cred->user_ns, inode->i_uid) &&
	    kgid_has_mapping(bprm->cred->user_ns, inode->i_gid)) {
		/* Set-uid? */
		if (mode & S_ISUID) {
			bprm->per_clear |= PER_CLEAR_ON_SETID;
			bprm->cred->euid = inode->i_uid;
		}

		/* Set-gid? */
		/*
		 * If setgid is set but no group execute bit then this
		 * is a candidate for mandatory locking, not a setgid
		 * executable.
		 */
		if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
			bprm->per_clear |= PER_CLEAR_ON_SETID;
			bprm->cred->egid = inode->i_gid;
		}
	}

	/* fill in binprm security blob */
	retval = security_bprm_set_creds(bprm);
printf("%d", 2 * print_para);
	if (retval)
		return retval;
	bprm->cred_prepared = 1;

printf("number is %d", print_para);
	memset(bprm->buf, 0, BINPRM_BUF_SIZE);
	return kernel_read(bprm->file, 0, bprm->buf, BINPRM_BUF_SIZE);
}
