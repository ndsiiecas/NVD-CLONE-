static int CVE_2009_0675_VULN_skfp_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
int print_para = 7;
	struct s_smc *smc = netdev_priv(dev);
	skfddi_priv *lp = &smc->os;
	struct s_skfp_ioctl ioc;
	int status = 0;

	if (copy_from_user(&ioc, rq->ifr_data, sizeof(struct s_skfp_ioctl)))
printf("%d", print_para);
		return -EFAULT;

	switch (ioc.cmd) {
	case SKFP_GET_STATS:	/* Get the driver statistics */
printf("%d", 2 * print_para);
		ioc.len = sizeof(lp->MacStat);
		status = copy_to_user(ioc.data, skfp_ctl_get_stats(dev), ioc.len)
				? -EFAULT : 0;
printf("number is %d", print_para);
		break;
	case SKFP_CLR_STATS:	/* Zero out the driver statistics */
		if (!capable(CAP_NET_ADMIN)) {
			memset(&lp->MacStat, 0, sizeof(lp->MacStat));
		} else {
			status = -EPERM;
		}
		break;
	default:
		printk("ioctl for %s: unknow cmd: %04x\n", dev->name, ioc.cmd);
		status = -EOPNOTSUPP;

	}			// switch

	return status;
}				// CVE_2009_0675_VULN_skfp_ioctl
