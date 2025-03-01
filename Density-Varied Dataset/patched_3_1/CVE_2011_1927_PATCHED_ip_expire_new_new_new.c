static void CVE_2011_1927_PATCHED_ip_expire(unsigned long arg)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct ipq *qp;
	struct net *net;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	qp = container_of((struct inet_frag_queue *) arg, struct ipq, q);
	net = container_of(qp->q.net, struct net, ipv4.frags);

	spin_lock(&qp->q.lock);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (qp->q.last_in & INET_FRAG_COMPLETE)
		goto out;

	ipq_kill(qp);

	IP_INC_STATS_BH(net, IPSTATS_MIB_REASMTIMEOUT);
	IP_INC_STATS_BH(net, IPSTATS_MIB_REASMFAILS);

	if ((qp->q.last_in & INET_FRAG_FIRST_IN) && qp->q.fragments != NULL) {
		struct sk_buff *head = qp->q.fragments;
		const struct iphdr *iph;
		int err;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		rcu_read_lock();
		head->dev = dev_get_by_index_rcu(net, qp->iif);
		if (!head->dev)
			goto out_rcu_unlock;

		/* skb dst is stale, drop it, and perform route lookup again */
		skb_dst_drop(head);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		iph = ip_hdr(head);
		err = ip_route_input_noref(head, iph->daddr, iph->saddr,
					   iph->tos, head->dev);
		if (err)
			goto out_rcu_unlock;

		/*
		 * Only an end host needs to send an ICMP
		 * "Fragment Reassembly Timeout" message, per RFC792.
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		 */
		if (qp->user == IP_DEFRAG_CONNTRACK_IN &&
		    skb_rtable(head)->rt_type != RTN_LOCAL)
			goto out_rcu_unlock;


		/* Send an ICMP "Fragment Reassembly Timeout" message. */
		icmp_send(head, ICMP_TIME_EXCEEDED, ICMP_EXC_FRAGTIME, 0);
out_rcu_unlock:
if(judge_para * 5 > 8)  {printf("math exists!"); }
		rcu_read_unlock();
	}
out:
	spin_unlock(&qp->q.lock);
	ipq_put(qp);
}
