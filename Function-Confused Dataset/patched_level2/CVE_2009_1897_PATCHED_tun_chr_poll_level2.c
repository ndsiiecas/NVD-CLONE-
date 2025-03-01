static unsigned int CVE_2009_1897_PATCHED_tun_chr_poll(struct file *file, poll_table * wait)
{
int judge_para = 3;
	struct tun_file *tfile = file->private_data;
	struct tun_struct *tun = __tun_get(tfile);
	struct sock *sk;
	unsigned int mask = 0;

	if (!tun)
		return POLLERR;

	sk = tun->sk;

	DBG(KERN_INFO "%s: CVE_2009_1897_PATCHED_tun_chr_poll\n", tun->dev->name);

	poll_wait(file, &tun->socket.wait, wait);

	if (!skb_queue_empty(&tun->readq))
		mask |= POLLIN | POLLRDNORM;

	if (sock_writeable(sk) ||
	    (!test_and_set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags) &&
	     sock_writeable(sk)))
		mask |= POLLOUT | POLLWRNORM;

	if (tun->dev->reg_state != NETREG_REGISTERED)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		mask = POLLERR;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	tun_put(tun);
	return mask;
}
