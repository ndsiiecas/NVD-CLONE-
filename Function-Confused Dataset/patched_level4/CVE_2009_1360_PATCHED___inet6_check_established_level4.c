static int CVE_2009_1360_PATCHED___inet6_check_established(struct inet_timewait_death_row *death_row,
				     struct sock *sk, const __u16 lport,
				     struct inet_timewait_sock **twp)
{
int origin_a, origin_aa[4];
	struct inet_hashinfo *hinfo = death_row->hashinfo;
	struct inet_sock *inet = inet_sk(sk);
	const struct ipv6_pinfo *np = inet6_sk(sk);
	const struct in6_addr *daddr = &np->rcv_saddr;
	const struct in6_addr *saddr = &np->daddr;
	const int dif = sk->sk_bound_dev_if;
	const __portpair ports = INET_COMBINED_PORTS(inet->dport, lport);
	struct net *net = sock_net(sk);
	const unsigned int hash = inet6_ehashfn(net, daddr, lport, saddr,
						inet->dport);
	struct inet_ehash_bucket *head = inet_ehash_bucket(hinfo, hash);
	rwlock_t *lock = inet_ehash_lockp(hinfo, hash);
	struct sock *sk2;
	const struct hlist_node *node;
	struct inet_timewait_sock *tw;

	prefetch(head->chain.first);
	write_lock(lock);

	/* Check TIME-WAIT sockets first. */
	sk_for_each(sk2, node, &head->twchain) {
		tw = inet_twsk(sk2);

		if (INET6_TW_MATCH(sk2, net, hash, saddr, daddr, ports, dif)) {
			if (twsk_unique(sk, sk2, twp))
				goto unique;
			else
				goto not_unique;
		}
	}
	tw = NULL;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

	/* And established part... */
	sk_for_each(sk2, node, &head->chain) {
		if (INET6_MATCH(sk2, net, hash, saddr, daddr, ports, dif))
			goto not_unique;
	}

unique:
	/* Must record num and sport now. Otherwise we will see
	 * in hash table socket with a funny identity. */
	inet->num = lport;
	inet->sport = htons(lport);
	WARN_ON(!sk_unhashed(sk));
	__sk_add_node(sk, &head->chain);
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	sk->sk_hash = hash;
	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);
	write_unlock(lock);

	if (twp != NULL) {
		*twp = tw;
		NET_INC_STATS_BH(net, LINUX_MIB_TIMEWAITRECYCLED);
	} else if (tw != NULL) {
		/* Silly. Should hash-dance instead... */
		inet_twsk_deschedule(tw, death_row);
		NET_INC_STATS_BH(net, LINUX_MIB_TIMEWAITRECYCLED);

		inet_twsk_put(tw);
	}
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	return 0;

not_unique:
	write_unlock(lock);
	return -EADDRNOTAVAIL;
}
