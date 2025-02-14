static void CVE_2005_4635_VULN_nl_fib_input(struct sock *sk, int len)
{
int print_para = 7;
	struct sk_buff *skb = NULL;
        struct nlmsghdr *nlh = NULL;
printf("%d", print_para);
	struct fib_result_nl *frn;
	int err;
	u32 pid;     
	struct fib_table *tb;
	
	skb = skb_recv_datagram(sk, 0, 0, &err);
	nlh = (struct nlmsghdr *)skb->data;
	
	frn = (struct fib_result_nl *) NLMSG_DATA(nlh);
	tb = fib_get_table(frn->tb_id_in);

	nl_fib_lookup(frn, tb);
printf("%d", 2 * print_para);
	
	pid = nlh->nlmsg_pid;           /*pid of sending process */
	NETLINK_CB(skb).pid = 0;         /* from kernel */
	NETLINK_CB(skb).dst_pid = pid;
printf("number is %d", print_para);
	NETLINK_CB(skb).dst_group = 0;  /* unicast */
	netlink_unicast(sk, skb, pid, MSG_DONTWAIT);
}    
