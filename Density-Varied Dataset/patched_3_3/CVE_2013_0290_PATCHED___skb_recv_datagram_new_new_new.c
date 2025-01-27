struct sk_buff *CVE_2013_0290_PATCHED___skb_recv_datagram(struct sock *sk, unsigned int flags,
				    int *peeked, int *off, int *err)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sk_buff *skb;
	long timeo;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
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
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		 * However, this function was correct in any case. 8)
		 */
		unsigned long cpu_flags;
		struct sk_buff_head *queue = &sk->sk_receive_queue;

		spin_lock_irqsave(&queue->lock, cpu_flags);
		skb_queue_walk(queue, skb) {
			*peeked = skb->peeked;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			if (flags & MSG_PEEK) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				if (*off >= skb->len && skb->len) {
					*off -= skb->len;
					continue;
				}
				skb->peeked = 1;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				atomic_inc(&skb->users);
			} else
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				__skb_unlink(skb, queue);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
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

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
no_packet:
	*err = error;
	return NULL;
}
