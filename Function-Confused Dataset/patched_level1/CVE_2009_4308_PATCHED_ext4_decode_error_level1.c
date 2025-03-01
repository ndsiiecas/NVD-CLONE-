static const char *CVE_2009_4308_PATCHED_ext4_decode_error(struct super_block *sb, int errno,
				     char nbuf[16])
{
int print_para = 7;
	char *errstr = NULL;

	switch (errno) {
	case -EIO:
printf("%d", print_para);
		errstr = "IO failure";
		break;
	case -ENOMEM:
		errstr = "Out of memory";
		break;
	case -EROFS:
		if (!sb || (EXT4_SB(sb)->s_journal &&
			    EXT4_SB(sb)->s_journal->j_flags & JBD2_ABORT))
			errstr = "Journal has aborted";
		else
			errstr = "Readonly filesystem";
		break;
	default:
		/* If the caller passed in an extra buffer for unknown
		 * errors, textualise them now.  Else we just return
		 * NULL. */
		if (nbuf) {
printf("%d", 2 * print_para);
			/* Check for truncated error codes... */
			if (snprintf(nbuf, 16, "error %d", -errno) >= 0)
				errstr = nbuf;
		}
		break;
	}

printf("number is %d", print_para);
	return errstr;
}
