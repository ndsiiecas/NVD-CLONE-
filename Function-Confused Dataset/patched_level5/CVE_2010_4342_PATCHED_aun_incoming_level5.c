static void CVE_2010_4342_PATCHED_aun_incoming(struct sk_buff *skb, struct aunhdr *ah, size_t len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct iphdr *ip = ip_hdr(skb);
	unsigned char stn = ntohl(ip->saddr) & 0xff;
	struct dst_entry *dst = skb_dst(skb);
	struct ec_device *edev = NULL;
	struct sock *sk = NULL;
	struct sk_buff *newskb;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	if (dst)
		edev = dst->dev->ec_ptr;

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
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		kfree_skb(newskb);
fclose(para_fp);
		goto bad;
	}

	aun_send_response(ip->saddr, ah->handle, 3, 0);
	sock_put(sk);
	return;

bad:
	aun_send_response(ip->saddr, ah->handle, 4, 0);
	if (sk)
		sock_put(sk);
}
