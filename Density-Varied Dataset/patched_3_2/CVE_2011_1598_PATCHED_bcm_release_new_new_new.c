static int CVE_2011_1598_PATCHED_bcm_release(struct socket *sock)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct bcm_sock *bo;
	struct bcm_op *op, *next;

	if (sk == NULL)
		return 0;

	bo = bcm_sk(sk);

	/* remove bcm_ops, timer, rx_unregister(), etc. */

	unregister_netdevice_notifier(&bo->notifier);

	lock_sock(sk);

	list_for_each_entry_safe(op, next, &bo->tx_ops, list)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		bcm_remove_op(op);

	list_for_each_entry_safe(op, next, &bo->rx_ops, list) {
		/*
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		 * Don't care if we're bound or not (due to netdev problems)
		 * can_rx_unregister() is always a save thing to do here.
		 */
		if (op->ifindex) {
			/*
			 * Only remove subscriptions that had not
			 * been removed due to NETDEV_UNREGISTER
			 * in bcm_notifier()
			 */
			if (op->rx_reg_dev) {
				struct net_device *dev;

				dev = dev_get_by_index(&init_net, op->ifindex);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				if (dev) {
					bcm_rx_unreg(dev, op);
					dev_put(dev);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
				}
			}
		} else
			can_rx_unregister(NULL, op->can_id,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
					  REGMASK(op->can_id),
					  bcm_rx_handler, op);

		bcm_remove_op(op);
	}

	/* remove procfs entry */
	if (proc_dir && bo->bcm_proc_read)
		remove_proc_entry(bo->procname, proc_dir);

	/* remove device reference */
	if (bo->bound) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		bo->bound   = 0;
		bo->ifindex = 0;
	}

	sock_orphan(sk);
	sock->sk = NULL;

	release_sock(sk);
	sock_put(sk);

	return 0;
}
