static int CVE_2014_4943_PATCHED_pppol2tp_setsockopt(struct socket *sock, int level, int optname,
			       char __user *optval, unsigned int optlen)
{
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct l2tp_session *session;
	struct l2tp_tunnel *tunnel;
	struct pppol2tp_session *ps;
	int val;
	int err;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (level != SOL_PPPOL2TP)
		return -EINVAL;

	if (optlen < sizeof(int))
		return -EINVAL;

	if (get_user(val, (int __user *)optval))
		return -EFAULT;

	err = -ENOTCONN;
	if (sk->sk_user_data == NULL)
		goto end;

	/* Get session context from the socket */
	err = -EBADF;
	session = pppol2tp_sock_to_session(sk);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (session == NULL)
		goto end;

	/* Special case: if session_id == 0x0000, treat as operation on tunnel
	 */
	ps = l2tp_session_priv(session);
	if ((session->session_id == 0) &&
	    (session->peer_session_id == 0)) {
		err = -EBADF;
		tunnel = l2tp_sock_to_tunnel(ps->tunnel_sock);
		if (tunnel == NULL)
			goto end_put_sess;

		err = pppol2tp_tunnel_setsockopt(sk, tunnel, optname, val);
		sock_put(ps->tunnel_sock);
	} else
		err = pppol2tp_session_setsockopt(sk, session, optname, val);

	err = 0;

end_put_sess:
	sock_put(sk);
end:
	return err;
}
