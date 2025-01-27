static int CVE_2009_1298_VULN_ip_frag_reasm(struct ipq *qp, struct sk_buff *prev,
			 struct net_device *dev)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct net *net = container_of(qp->q.net, struct net, ipv4.frags);
	struct iphdr *iph;
	struct sk_buff *fp, *head = qp->q.fragments;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	int len;
	int ihlen;
	int err;

	ipq_kill(qp);

	/* Make the one we just received the head. */
	if (prev) {
		head = prev->next;
		fp = skb_clone(head, GFP_ATOMIC);
		if (!fp)
			goto out_nomem;

		fp->next = head->next;
		prev->next = fp;

		skb_morph(head, qp->q.fragments);
		head->next = qp->q.fragments->next;

		kfree_skb(qp->q.fragments);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		qp->q.fragments = head;
	}

	WARN_ON(head == NULL);
	WARN_ON(FRAG_CB(head)->offset != 0);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	/* Allocate a new buffer for the datagram. */
	ihlen = ip_hdrlen(head);
	len = ihlen + qp->q.len;

	err = -E2BIG;
	if (len > 65535)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		goto out_oversize;

	/* Head of list must not be cloned. */
	if (skb_cloned(head) && pskb_expand_head(head, 0, 0, GFP_ATOMIC))
		goto out_nomem;

	/* If the first fragment is fragmented itself, we split
	 * it to two chunks: the first with data and paged part
	 * and the second, holding only fragments. */
	if (skb_shinfo(head)->frag_list) {
		struct sk_buff *clone;
		int i, plen = 0;

		if ((clone = alloc_skb(0, GFP_ATOMIC)) == NULL)
			goto out_nomem;
		clone->next = head->next;
		head->next = clone;
		skb_shinfo(clone)->frag_list = skb_shinfo(head)->frag_list;
		skb_shinfo(head)->frag_list = NULL;
		for (i=0; i<skb_shinfo(head)->nr_frags; i++)
			plen += skb_shinfo(head)->frags[i].size;
		clone->len = clone->data_len = head->data_len - plen;
		head->data_len -= clone->len;
		head->len -= clone->len;
		clone->csum = 0;
		clone->ip_summed = head->ip_summed;
		atomic_add(clone->truesize, &qp->q.net->mem);
	}

	skb_shinfo(head)->frag_list = head->next;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	skb_push(head, head->data - skb_network_header(head));
	atomic_sub(head->truesize, &qp->q.net->mem);

	for (fp=head->next; fp; fp = fp->next) {
		head->data_len += fp->len;
		head->len += fp->len;
		if (head->ip_summed != fp->ip_summed)
			head->ip_summed = CHECKSUM_NONE;
		else if (head->ip_summed == CHECKSUM_COMPLETE)
			head->csum = csum_add(head->csum, fp->csum);
		head->truesize += fp->truesize;
		atomic_sub(fp->truesize, &qp->q.net->mem);
	}
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	head->next = NULL;
	head->dev = dev;
	head->tstamp = qp->q.stamp;

	iph = ip_hdr(head);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	iph->frag_off = 0;
	iph->tot_len = htons(len);
	IP_INC_STATS_BH(net, IPSTATS_MIB_REASMOKS);
	qp->q.fragments = NULL;
	return 0;

out_nomem:
	LIMIT_NETDEBUG(KERN_ERR "IP: queue_glue: no memory for gluing "
			      "queue %p\n", qp);
	err = -ENOMEM;
	goto out_fail;
out_oversize:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (net_ratelimit())
		printk(KERN_INFO "Oversized IP packet from %pI4.\n",
			&qp->saddr);
out_fail:
	IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_REASMFAILS);
	return err;
}
