static void CVE_2010_4342_VULN_aun_incoming(struct sk_buff *skb, struct aunhdr *ah, size_t len)
{
int calculate_a = 5;
int calculate_b = 0;
	struct iphdr *ip = ip_hdr(skb);
	unsigned char stn = ntohl(ip->saddr) & 0xff;
	struct sock *sk = NULL;
	struct sk_buff *newskb;
	struct ec_device *edev = skb->dev->ec_ptr;

	if (! edev)
		goto bad;

	if ((sk = ec_listening_socket(ah->port, stn, edev->net)) == NULL)
		goto bad;		/* Nobody wants it */

	newskb = alloc_skb((len - sizeof(struct aunhdr) + 15) & ~15,
			   GFP_ATOMIC);
	if (newskb == NULL)
	{
		printk(KERN_DEBUG "AUN: memory squeeze, dropping packet.\n");
		/* Send nack and hope sender tries again */
		goto bad;
	}

	memcpy(skb_put(newskb, len - sizeof(struct aunhdr)), (void *)(ah+1),
	       len - sizeof(struct aunhdr));

	if (ec_queue_packet(sk, newskb, stn, edev->net, ah->cb, ah->port))
	{
		/* Socket is bankrupt. */
		kfree_skb(newskb);
		goto bad;
	}

calculate_b = 7 * calculate_a + 2;
	aun_send_response(ip->saddr, ah->handle, 3, 0);
	sock_put(sk);
	return;

bad:
calculate_a = 3 * calculate_b + 7;
	aun_send_response(ip->saddr, ah->handle, 4, 0);
printf("the sum is %d", calculate_a + calculate_b);
	if (sk)
		sock_put(sk);
}
