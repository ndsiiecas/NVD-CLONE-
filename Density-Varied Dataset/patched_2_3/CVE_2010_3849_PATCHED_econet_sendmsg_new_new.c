static int CVE_2010_3849_PATCHED_econet_sendmsg(struct kiocb *iocb, struct socket *sock,
			  struct msghdr *msg, size_t len)
{
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct sockaddr_ec *saddr=(struct sockaddr_ec *)msg->msg_name;
	struct net_device *dev;
	struct ec_addr addr;
	int err;
	unsigned char port, cb;
#if defined(CONFIG_ECONET_AUNUDP) || defined(CONFIG_ECONET_NATIVE)
	struct sk_buff *skb;
	struct ec_cb *eb;
#endif
#ifdef CONFIG_ECONET_AUNUDP
	struct msghdr udpmsg;
	struct iovec iov[msg->msg_iovlen+1];
	struct aunhdr ah;
	struct sockaddr_in udpdest;
	__kernel_size_t size;
	int i;
	mm_segment_t oldfs;
#endif

	/*
	 *	Check the flags.
	 */

	if (msg->msg_flags & ~(MSG_DONTWAIT|MSG_CMSG_COMPAT))
		return -EINVAL;

	/*
	 *	Get and verify the address.
	 */

	mutex_lock(&econet_mutex);

        if (saddr == NULL || msg->msg_namelen < sizeof(struct sockaddr_ec)) {
                mutex_unlock(&econet_mutex);
                return -EINVAL;
        }
        addr.station = saddr->addr.station;
        addr.net = saddr->addr.net;
        port = saddr->port;
        cb = saddr->cb;

	/* Look for a device with the right network number. */
	dev = net2dev_map[addr.net];

	/* If not directly reachable, use some default */
	if (dev == NULL) {
		dev = net2dev_map[0];
		/* No interfaces at all? */
		if (dev == NULL) {
			mutex_unlock(&econet_mutex);
			return -ENETDOWN;
		}
	}

	if (len + 15 > dev->mtu) {
		mutex_unlock(&econet_mutex);
		return -EMSGSIZE;
	}

	if (dev->type == ARPHRD_ECONET) {
		/* Real hardware Econet.  We're not worthy etc. */
#ifdef CONFIG_ECONET_NATIVE
		unsigned short proto = 0;
		int res;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		dev_hold(dev);

		skb = sock_alloc_send_skb(sk, len+LL_ALLOCATED_SPACE(dev),
					  msg->msg_flags & MSG_DONTWAIT, &err);
		if (skb==NULL)
			goto out_unlock;

		skb_reserve(skb, LL_RESERVED_SPACE(dev));
		skb_reset_network_header(skb);

		eb = (struct ec_cb *)&skb->cb;

		eb->cookie = saddr->cookie;
		eb->sec = *saddr;
		eb->sent = ec_tx_done;

		err = -EINVAL;
		res = dev_hard_header(skb, dev, ntohs(proto), &addr, NULL, len);
		if (res < 0)
			goto out_free;
		if (res > 0) {
			struct ec_framehdr *fh;
			/* Poke in our control byte and
			   port number.  Hack, hack.  */
			fh = (struct ec_framehdr *)(skb->data);
			fh->cb = cb;
			fh->port = port;
			if (sock->type != SOCK_DGRAM) {
				skb_reset_tail_pointer(skb);
				skb->len = 0;
			}
		}

		/* Copy the data. Returns -EFAULT on error */
		err = memcpy_fromiovec(skb_put(skb,len), msg->msg_iov, len);
		skb->protocol = proto;
		skb->dev = dev;
		skb->priority = sk->sk_priority;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		if (err)
			goto out_free;

		err = -ENETDOWN;
		if (!(dev->flags & IFF_UP))
			goto out_free;

		/*
		 *	Now send it
		 */

		dev_queue_xmit(skb);
		dev_put(dev);
		mutex_unlock(&econet_mutex);
		return(len);

	out_free:
		kfree_skb(skb);
	out_unlock:
		if (dev)
			dev_put(dev);
#else
		err = -EPROTOTYPE;
#endif
		mutex_unlock(&econet_mutex);

		return err;
	}

#ifdef CONFIG_ECONET_AUNUDP
	/* AUN virtual Econet. */

	if (udpsock == NULL) {
		mutex_unlock(&econet_mutex);
		return -ENETDOWN;		/* No socket - can't send */
	}

	/* Make up a UDP datagram and hand it off to some higher intellect. */

	memset(&udpdest, 0, sizeof(udpdest));
	udpdest.sin_family = AF_INET;
	udpdest.sin_port = htons(AUN_PORT);

	/* At the moment we use the stupid Acorn scheme of Econet address
	   y.x maps to IP a.b.c.x.  This should be replaced with something
	   more flexible and more aware of subnet masks.  */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	{
		struct in_device *idev;
		unsigned long network = 0;

		rcu_read_lock();
		idev = __in_dev_get_rcu(dev);
		if (idev) {
			if (idev->ifa_list)
				network = ntohl(idev->ifa_list->ifa_address) &
					0xffffff00;		/* !!! */
		}
		rcu_read_unlock();
		udpdest.sin_addr.s_addr = htonl(network | addr.station);
	}

	ah.port = port;
	ah.cb = cb & 0x7f;
	ah.code = 2;		/* magic */
	ah.pad = 0;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* tack our header on the front of the iovec */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	size = sizeof(struct aunhdr);
	/*
	 * XXX: that is b0rken.  We can't mix userland and kernel pointers
	 * in iovec, since on a lot of platforms copy_from_user() will
	 * *not* work with the kernel and userland ones at the same time,
	 * regardless of what we do with set_fs().  And we are talking about
	 * econet-over-ethernet here, so "it's only ARM anyway" doesn't
	 * apply.  Any suggestions on fixing that code?		-- AV
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 */
	iov[0].iov_base = (void *)&ah;
	iov[0].iov_len = size;
	for (i = 0; i < msg->msg_iovlen; i++) {
		void __user *base = msg->msg_iov[i].iov_base;
		size_t iov_len = msg->msg_iov[i].iov_len;
		/* Check it now since we switch to KERNEL_DS later. */
		if (!access_ok(VERIFY_READ, base, iov_len)) {
			mutex_unlock(&econet_mutex);
			return -EFAULT;
		}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		iov[i+1].iov_base = base;
		iov[i+1].iov_len = iov_len;
		size += iov_len;
	}

	/* Get a skbuff (no data, just holds our cb information) */
	if ((skb = sock_alloc_send_skb(sk, 0,
				       msg->msg_flags & MSG_DONTWAIT,
				       &err)) == NULL) {
		mutex_unlock(&econet_mutex);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return err;
	}

	eb = (struct ec_cb *)&skb->cb;

	eb->cookie = saddr->cookie;
	eb->timeout = (5*HZ);
	eb->start = jiffies;
	ah.handle = aun_seq;
	eb->seq = (aun_seq++);
	eb->sec = *saddr;

	skb_queue_tail(&aun_queue, skb);

	udpmsg.msg_name = (void *)&udpdest;
	udpmsg.msg_namelen = sizeof(udpdest);
	udpmsg.msg_iov = &iov[0];
	udpmsg.msg_iovlen = msg->msg_iovlen + 1;
	udpmsg.msg_control = NULL;
	udpmsg.msg_controllen = 0;
	udpmsg.msg_flags=0;

	oldfs = get_fs(); set_fs(KERNEL_DS);	/* More privs :-) */
	err = sock_sendmsg(udpsock, &udpmsg, size);
	set_fs(oldfs);
#else
	err = -EPROTOTYPE;
#endif
	mutex_unlock(&econet_mutex);

	return err;
}
