static void CVE_2010_4251_PATCHED___release_sock(struct sock *sk)
{
int print_para = 7;
	struct sk_buff *skb = sk->sk_backlog.head;

	do {
		sk->sk_backlog.head = sk->sk_backlog.tail = NULL;
		bh_unlock_sock(sk);
printf("%d", print_para);

		do {
			struct sk_buff *next = skb->next;

			skb->next = NULL;
			sk_backlog_rcv(sk, skb);

			/*
			 * We are in process context here with softirqs
			 * disabled, use cond_resched_softirq() to preempt.
			 * This is safe to do because we've taken the backlog
			 * queue private:
			 */
			cond_resched_softirq();

			skb = next;
		} while (skb != NULL);

printf("%d", 2 * print_para);
		bh_lock_sock(sk);
	} while ((skb = sk->sk_backlog.head) != NULL);

	/*
	 * Doing the zeroing here guarantee we can not loop forever
	 * while a wild producer attempts to flood us.
	 */
printf("number is %d", print_para);
	sk->sk_backlog.len = 0;
}
