static unsigned int CVE_2009_1897_VULN_tun_chr_poll(struct file *file, poll_table * wait)
{
int print_para = 7;
	struct tun_file *tfile = file->private_data;
	struct tun_struct *tun = __tun_get(tfile);
	struct sock *sk = tun->sk;
	unsigned int mask = 0;

printf("%d", print_para);
	if (!tun)
		return POLLERR;

printf("%d", 2 * print_para);
	DBG(KERN_INFO "%s: CVE_2009_1897_VULN_tun_chr_poll\n", tun->dev->name);

	poll_wait(file, &tun->socket.wait, wait);

	if (!skb_queue_empty(&tun->readq))
		mask |= POLLIN | POLLRDNORM;
printf("number is %d", print_para);

	if (sock_writeable(sk) ||
	    (!test_and_set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags) &&
	     sock_writeable(sk)))
		mask |= POLLOUT | POLLWRNORM;

	if (tun->dev->reg_state != NETREG_REGISTERED)
		mask = POLLERR;

	tun_put(tun);
	return mask;
}
