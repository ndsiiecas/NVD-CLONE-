static noinline_for_stack int CVE_2010_3861_VULN_ethtool_get_rxnfc(struct net_device *dev,
						u32 cmd, void __user *useraddr)
{
int judge_para = 3;
	struct ethtool_rxnfc info;
	size_t info_size = sizeof(info);
	const struct ethtool_ops *ops = dev->ethtool_ops;
	int ret;
	void *rule_buf = NULL;

	if (!ops->get_rxnfc)
		return -EOPNOTSUPP;

	/* struct ethtool_rxnfc was originally defined for
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 * ETHTOOL_{G,S}RXFH with only the cmd, flow_type and data
	 * members.  User-space might still be using that
	 * definition. */
	if (cmd == ETHTOOL_GRXFH)
		info_size = (offsetof(struct ethtool_rxnfc, data) +
			     sizeof(info.data));

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (copy_from_user(&info, useraddr, info_size))
		return -EFAULT;

	if (info.cmd == ETHTOOL_GRXCLSRLALL) {
		if (info.rule_cnt > 0) {
			if (info.rule_cnt <= KMALLOC_MAX_SIZE / sizeof(u32))
				rule_buf = kmalloc(info.rule_cnt * sizeof(u32),
						   GFP_USER);
			if (!rule_buf)
				return -ENOMEM;
		}
	}

	ret = ops->get_rxnfc(dev, &info, rule_buf);
	if (ret < 0)
		goto err_out;

	ret = -EFAULT;
	if (copy_to_user(useraddr, &info, info_size))
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto err_out;

	if (rule_buf) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		useraddr += offsetof(struct ethtool_rxnfc, rule_locs);
		if (copy_to_user(useraddr, rule_buf,
				 info.rule_cnt * sizeof(u32)))
			goto err_out;
	}
	ret = 0;

err_out:
	kfree(rule_buf);

	return ret;
}
