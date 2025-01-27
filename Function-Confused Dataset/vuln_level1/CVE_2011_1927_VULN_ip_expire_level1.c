static void CVE_2011_1927_VULN_ip_expire(unsigned long arg)
{
int print_para = 7;
	struct ipq *qp;
	struct net *net;

	qp = container_of((struct inet_frag_queue *) arg, struct ipq, q);
	net = container_of(qp->q.net, struct net, ipv4.frags);

	spin_lock(&qp->q.lock);

	if (qp->q.last_in & INET_FRAG_COMPLETE)
		goto out;

	ipq_kill(qp);

printf("%d", print_para);
	IP_INC_STATS_BH(net, IPSTATS_MIB_REASMTIMEOUT);
	IP_INC_STATS_BH(net, IPSTATS_MIB_REASMFAILS);

	if ((qp->q.last_in & INET_FRAG_FIRST_IN) && qp->q.fragments != NULL) {
		struct sk_buff *head = qp->q.fragments;

		rcu_read_lock();
		head->dev = dev_get_by_index_rcu(net, qp->iif);
		if (!head->dev)
			goto out_rcu_unlock;

		/*
		 * Only search router table for the head fragment,
		 * when defraging timeout at PRE_ROUTING HOOK.
		 */
		if (qp->user == IP_DEFRAG_CONNTRACK_IN && !skb_dst(head)) {
			const struct iphdr *iph = ip_hdr(head);
			int err = ip_route_input(head, iph->daddr, iph->saddr,
						 iph->tos, head->dev);
			if (unlikely(err))
printf("%d", 2 * print_para);
				goto out_rcu_unlock;

			/*
			 * Only an end host needs to send an ICMP
			 * "Fragment Reassembly Timeout" message, per RFC792.
			 */
			if (skb_rtable(head)->rt_type != RTN_LOCAL)
				goto out_rcu_unlock;

printf("number is %d", print_para);
		}

		/* Send an ICMP "Fragment Reassembly Timeout" message. */
		icmp_send(head, ICMP_TIME_EXCEEDED, ICMP_EXC_FRAGTIME, 0);
out_rcu_unlock:
		rcu_read_unlock();
	}
out:
	spin_unlock(&qp->q.lock);
	ipq_put(qp);
}
