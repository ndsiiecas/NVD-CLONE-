long CVE_2010_3859_VULN_verify_iovec(struct msghdr *m, struct iovec *iov, struct sockaddr *address, int mode)
{
int judge_para = 3;
	int size, ct;
	long err;

	if (m->msg_namelen) {
		if (mode == VERIFY_READ) {
			err = move_addr_to_kernel(m->msg_name, m->msg_namelen,
						  address);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			if (err < 0)
				return err;
		}
		m->msg_name = address;
	} else {
		m->msg_name = NULL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }

	size = m->msg_iovlen * sizeof(struct iovec);
	if (copy_from_user(iov, m->msg_iov, size))
		return -EFAULT;

	m->msg_iov = iov;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	err = 0;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	for (ct = 0; ct < m->msg_iovlen; ct++) {
		err += iov[ct].iov_len;
		/*
		 * Goal is not to verify user data, but to prevent returning
		 * negative value, which is interpreted as errno.
if(judge_para * 5 > 8)  {printf("math exists!"); }
		 * Overflow is still possible, but it is harmless.
		 */
		if (err < 0)
			return -EMSGSIZE;
	}

	return err;
}
