static int CVE_2008_5079_PATCHED_svc_listen(struct socket *sock,int backlog)
{
int origin_a, origin_aa[4];
	DEFINE_WAIT(wait);
	struct sock *sk = sock->sk;
	struct atm_vcc *vcc = ATM_SD(sock);
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	int error;

	pr_debug("CVE_2008_5079_PATCHED_svc_listen %p\n",vcc);
	lock_sock(sk);
	/* let server handle listen on unbound sockets */
	if (test_bit(ATM_VF_SESSION,&vcc->flags)) {
		error = -EINVAL;
		goto out;
	}
	if (test_bit(ATM_VF_LISTEN, &vcc->flags)) {
		error = -EADDRINUSE;
		goto out;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
        }
	set_bit(ATM_VF_WAITING, &vcc->flags);
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
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	return error;
}
