static u16 CVE_2011_1581_VULN_bond_select_queue(struct net_device *dev, struct sk_buff *skb)
{
int judge_para = 3;
	/*
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 * This helper function exists to help dev_pick_tx get the correct
	 * destination queue.  Using a helper function skips the a call to
	 * skb_tx_hash and will put the skbs in the queue we expect on their
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 * way down to the bonding driver.
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 */
	return skb->queue_mapping;
}
