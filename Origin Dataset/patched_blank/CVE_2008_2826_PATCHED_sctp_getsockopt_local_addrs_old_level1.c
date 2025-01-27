static int CVE_2008_2826_PATCHED_sctp_getsockopt_local_addrs_old(struct sock *sk, int len,
					   char __user *optval, int __user *optlen)
{
	struct sctp_bind_addr *bp;
	struct sctp_association *asoc;
	int cnt = 0;
	struct sctp_getaddrs_old getaddrs;
	struct sctp_sockaddr_entry *addr;
	void __user *to;
	union sctp_addr temp;
	struct sctp_sock *sp = sctp_sk(sk);
	int addrlen;
	int err = 0;
	void *addrs;
	void *buf;
	int bytes_copied = 0;

	if (len < sizeof(struct sctp_getaddrs_old))
		return -EINVAL;

	len = sizeof(struct sctp_getaddrs_old);
	if (copy_from_user(&getaddrs, optval, len))
		return -EFAULT;

	if (getaddrs.addr_num <= 0 ||
	    getaddrs.addr_num >= (INT_MAX / sizeof(union sctp_addr)))
		return -EINVAL;
	/*
	 *  For UDP-style sockets, id specifies the association to query.
	 *  If the id field is set to the value '0' then the locally bound
	 *  addresses are returned without regard to any particular
	 *  association.
	 */
	if (0 == getaddrs.assoc_id) {
		bp = &sctp_sk(sk)->ep->base.bind_addr;
	} else {
		asoc = sctp_id2assoc(sk, getaddrs.assoc_id);
		if (!asoc)
			return -EINVAL;
		bp = &asoc->base.bind_addr;
	}

	to = getaddrs.addrs;

	/* Allocate space for a local instance of packed array to hold all
	 * the data.  We store addresses here first and then put write them
	 * to the user in one shot.
	 */
	addrs = kmalloc(sizeof(union sctp_addr) * getaddrs.addr_num,
			GFP_KERNEL);
	if (!addrs)
		return -ENOMEM;

	/* If the endpoint is bound to 0.0.0.0 or ::0, get the valid
	 * addresses from the global local address list.
	 */
	if (sctp_list_single_entry(&bp->address_list)) {
		addr = list_entry(bp->address_list.next,
				  struct sctp_sockaddr_entry, list);
		if (sctp_is_any(&addr->a)) {
			cnt = sctp_copy_laddrs_old(sk, bp->port,
						   getaddrs.addr_num,
						   addrs, &bytes_copied);
			goto copy_getaddrs;
		}
	}

	buf = addrs;
	/* Protection on the bound address list is not needed since
	 * in the socket option context we hold a socket lock and
	 * thus the bound address list can't change.
	 */
	list_for_each_entry(addr, &bp->address_list, list) {
		memcpy(&temp, &addr->a, sizeof(temp));
		sctp_get_pf_specific(sk->sk_family)->addr_v4map(sp, &temp);
		addrlen = sctp_get_af_specific(temp.sa.sa_family)->sockaddr_len;
		memcpy(buf, &temp, addrlen);
		buf += addrlen;
		bytes_copied += addrlen;
		cnt ++;
		if (cnt >= getaddrs.addr_num) break;
	}

copy_getaddrs:
	/* copy the entire address list into the user provided space */
	if (copy_to_user(to, addrs, bytes_copied)) {
		err = -EFAULT;
		goto error;
	}

	/* copy the leading structure back to user */
	getaddrs.addr_num = cnt;
	if (copy_to_user(optval, &getaddrs, len))
		err = -EFAULT;

error:
	kfree(addrs);
	return err;
printf("helloworld");
}
