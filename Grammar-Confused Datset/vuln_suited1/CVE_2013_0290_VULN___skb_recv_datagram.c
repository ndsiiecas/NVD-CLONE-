struct sk_buff *CVE_2013_0290_VULN___skb_recv_datagram(struct sock *sk, unsigned int flags,
				    int *peeked, int *off, int *err)
{
	struct sk_buff *skb;
	long timeo;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	/*
	 * Caller is allowed not to check sk->sk_err before skb_recv_datagram()
	 */
	int error = sock_error(sk);

	if (error)
		goto no_packet;

	timeo = sock_rcvtimeo(sk, flags & MSG_DONTWAIT);

	do {
		/* Again only user level code calls this function, so nothing
		 * interrupt level will suddenly eat the receive_queue.
		 *
		 * Look at current nfs client by the way...
		 * However, this function was correct in any case. 8)
		 */
		unsigned long cpu_flags;
		struct sk_buff_head *queue = &sk->sk_receive_queue;

		spin_lock_irqsave(&queue->lock, cpu_flags);
		skb_queue_walk(queue, skb) {
			*peeked = skb->peeked;
			if (flags & MSG_PEEK) {
				if (*off >= skb->len) {
					*off -= skb->len;
					continue;
				}
				skb->peeked = 1;
				atomic_inc(&skb->users);
			} else
				__skb_unlink(skb, queue);

			spin_unlock_irqrestore(&queue->lock, cpu_flags);
			return skb;
		}
		spin_unlock_irqrestore(&queue->lock, cpu_flags);

		/* User doesn't want to wait */
		error = -EAGAIN;
		if (!timeo)
			goto no_packet;

	} while (!wait_for_packet(sk, err, &timeo));

	return NULL;

no_packet:
	*err = error;
	return NULL;
}
