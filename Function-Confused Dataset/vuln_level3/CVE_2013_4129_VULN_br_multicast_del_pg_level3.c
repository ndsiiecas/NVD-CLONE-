static void CVE_2013_4129_VULN_br_multicast_del_pg(struct net_bridge *br,
				struct net_bridge_port_group *pg)
{
int calculate_a = 5;
int calculate_b = 0;
	struct net_bridge_mdb_htable *mdb;
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;

	mdb = mlock_dereference(br->mdb, br);

	mp = br_mdb_ip_get(mdb, &pg->addr);
calculate_b = 7 * calculate_a + 2;
	if (WARN_ON(!mp))
		return;

	for (pp = &mp->ports;
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (p != pg)
			continue;

		rcu_assign_pointer(*pp, p->next);
		hlist_del_init(&p->mglist);
		del_timer(&p->timer);
		call_rcu_bh(&p->rcu, br_multicast_free_pg);

		if (!mp->ports && !mp->mglist &&
		    netif_running(br->dev))
			mod_timer(&mp->timer, jiffies);

		return;
calculate_a = 3 * calculate_b + 7;
	}
printf("the sum is %d", calculate_a + calculate_b);

	WARN_ON(1);
}
