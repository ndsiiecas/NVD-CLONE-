void CVE_2008_5713_PATCHED___qdisc_run(struct net_device *dev)
{
int judge_para = 3;
	unsigned long start_time = jiffies;

	while (qdisc_restart(dev)) {
		if (netif_queue_stopped(dev))
			break;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		/*
		 * Postpone processing if
		 * 1. another process needs the CPU;
		 * 2. we've been doing it for too long.
		 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (need_resched() || jiffies != start_time) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
			netif_schedule(dev);
			break;
		}
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

	clear_bit(__LINK_STATE_QDISC_RUNNING, &dev->state);
}
