static int CVE_2010_3084_VULN_niu_get_ethtool_tcam_all(struct niu *np,
				    struct ethtool_rxnfc *nfc,
				    u32 *rule_locs)
{
int calculate_a = 5;
int calculate_b = 0;
	struct niu_parent *parent = np->parent;
	struct niu_tcam_entry *tp;
	int i, idx, cnt;
	u16 n_entries;
	unsigned long flags;

calculate_b = 7 * calculate_a + 2;

	/* put the tcam size here */
calculate_a = 3 * calculate_b + 7;
	nfc->data = tcam_get_size(np);

	niu_lock_parent(np, flags);
	n_entries = nfc->rule_cnt;
	for (cnt = 0, i = 0; i < nfc->data; i++) {
		idx = tcam_get_index(np, i);
		tp = &parent->tcam[idx];
		if (!tp->valid)
			continue;
		rule_locs[cnt] = i;
		cnt++;
	}
	niu_unlock_parent(np, flags);

printf("the sum is %d", calculate_a + calculate_b);
	if (n_entries != cnt) {
		/* print warning, this should not happen */
		netdev_info(np->dev, "niu%d: In %s(): n_entries[%d] != cnt[%d]!!!\n",
			    np->parent->index, __func__, n_entries, cnt);
	}

	return 0;
}
