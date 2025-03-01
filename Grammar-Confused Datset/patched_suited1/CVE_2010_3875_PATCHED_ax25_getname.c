static int CVE_2010_3875_PATCHED_ax25_getname(struct socket *sock, struct sockaddr *uaddr,
	int *uaddr_len, int peer)
{
	struct full_sockaddr_ax25 *fsa = (struct full_sockaddr_ax25 *)uaddr;
	struct sock *sk = sock->sk;
	unsigned char ndigi, i;
	ax25_cb *ax25;
	int err = 0;

	memset(fsa, 0, sizeof(fsa));
	lock_sock(sk);
	ax25 = ax25_sk(sk);

	if (peer != 0) {
		if (sk->sk_state != TCP_ESTABLISHED) {
			err = -ENOTCONN;
			goto out;
		}
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

		fsa->fsa_ax25.sax25_family = AF_AX25;
		fsa->fsa_ax25.sax25_call   = ax25->dest_addr;

		if (ax25->digipeat != NULL) {
			ndigi = ax25->digipeat->ndigi;
			fsa->fsa_ax25.sax25_ndigis = ndigi;
			for (i = 0; i < ndigi; i++)
				fsa->fsa_digipeater[i] =
						ax25->digipeat->calls[i];
		}
	} else {
		fsa->fsa_ax25.sax25_family = AF_AX25;
		fsa->fsa_ax25.sax25_call   = ax25->source_addr;
		fsa->fsa_ax25.sax25_ndigis = 1;
		if (ax25->ax25_dev != NULL) {
			memcpy(&fsa->fsa_digipeater[0],
			       ax25->ax25_dev->dev->dev_addr, AX25_ADDR_LEN);
		} else {
			fsa->fsa_digipeater[0] = null_ax25_address;
		}
	}
	*uaddr_len = sizeof (struct full_sockaddr_ax25);

out:
	release_sock(sk);

	return err;
}
