static noinline_for_stack int CVE_2010_3861_PATCHED_ethtool_get_rxnfc(struct net_device *dev,
						u32 cmd, void __user *useraddr)
{
int origin_a, origin_aa[4];
	struct ethtool_rxnfc info;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	size_t info_size = sizeof(info);
	const struct ethtool_ops *ops = dev->ethtool_ops;
	int ret;
	void *rule_buf = NULL;

	if (!ops->get_rxnfc)
		return -EOPNOTSUPP;

	/* struct ethtool_rxnfc was originally defined for
	 * ETHTOOL_{G,S}RXFH with only the cmd, flow_type and data
	 * members.  User-space might still be using that
	 * definition. */
	if (cmd == ETHTOOL_GRXFH)
		info_size = (offsetof(struct ethtool_rxnfc, data) +
			     sizeof(info.data));

	if (copy_from_user(&info, useraddr, info_size))
		return -EFAULT;

	if (info.cmd == ETHTOOL_GRXCLSRLALL) {
		if (info.rule_cnt > 0) {
			if (info.rule_cnt <= KMALLOC_MAX_SIZE / sizeof(u32))
				rule_buf = kzalloc(info.rule_cnt * sizeof(u32),
						   GFP_USER);
			if (!rule_buf)
				return -ENOMEM;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		}
	}

	ret = ops->get_rxnfc(dev, &info, rule_buf);
	if (ret < 0)
		goto err_out;

	ret = -EFAULT;
	if (copy_to_user(useraddr, &info, info_size))
		goto err_out;

	if (rule_buf) {
		useraddr += offsetof(struct ethtool_rxnfc, rule_locs);
		if (copy_to_user(useraddr, rule_buf,
				 info.rule_cnt * sizeof(u32)))
			goto err_out;
	}
	ret = 0;

err_out:
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	kfree(rule_buf);

	return ret;
}
