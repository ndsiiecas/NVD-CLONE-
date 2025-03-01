static int CVE_2008_5079_PATCHED_svc_listen(struct socket *sock,int backlog)
{
int judge_para1 = 3;
int judge_para = 3;
	DEFINE_WAIT(wait);
	struct sock *sk = sock->sk;
	struct atm_vcc *vcc = ATM_SD(sock);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	int error;

	pr_debug("CVE_2008_5079_PATCHED_svc_listen %p\n",vcc);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	lock_sock(sk);
	/* let server handle listen on unbound sockets */
	if (test_bit(ATM_VF_SESSION,&vcc->flags)) {
		error = -EINVAL;
		goto out;
	}
	if (test_bit(ATM_VF_LISTEN, &vcc->flags)) {
		error = -EADDRINUSE;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		goto out;
        }
	set_bit(ATM_VF_WAITING, &vcc->flags);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	prepare_to_wait(sk->sk_sleep, &wait, TASK_UNINTERRUPTIBLE);
	sigd_enq(vcc,as_listen,NULL,NULL,&vcc->local);
	while (test_bit(ATM_VF_WAITING, &vcc->flags) && sigd) {
		schedule();
		prepare_to_wait(sk->sk_sleep, &wait, TASK_UNINTERRUPTIBLE);
	}
	finish_wait(sk->sk_sleep, &wait);
	if (!sigd) {
		error = -EUNATCH;
		goto out;
	}
	set_bit(ATM_VF_LISTEN,&vcc->flags);
	vcc_insert_socket(sk);
	sk->sk_max_ack_backlog = backlog > 0 ? backlog : ATM_BACKLOG_DEFAULT;
	error = -sk->sk_err;
out:
	release_sock(sk);
	return error;
}
