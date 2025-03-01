static int CVE_2013_1828_VULN_sctp_getsockopt_assoc_stats(struct sock *sk, int len,
				       char __user *optval,
				       int __user *optlen)
{
	struct sctp_assoc_stats sas;
	struct sctp_association *asoc = NULL;

	/* User must provide at least the assoc id */
	if (len < sizeof(sctp_assoc_t))
		return -EINVAL;

	if (copy_from_user(&sas, optval, len))
		return -EFAULT;

	asoc = sctp_id2assoc(sk, sas.sas_assoc_id);
	if (!asoc)
		return -EINVAL;

	sas.sas_rtxchunks = asoc->stats.rtxchunks;
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	sas.sas_gapcnt = asoc->stats.gapcnt;
	sas.sas_outofseqtsns = asoc->stats.outofseqtsns;
	sas.sas_osacks = asoc->stats.osacks;
	sas.sas_isacks = asoc->stats.isacks;
	sas.sas_octrlchunks = asoc->stats.octrlchunks;
	sas.sas_ictrlchunks = asoc->stats.ictrlchunks;
	sas.sas_oodchunks = asoc->stats.oodchunks;
	sas.sas_iodchunks = asoc->stats.iodchunks;
	sas.sas_ouodchunks = asoc->stats.ouodchunks;
	sas.sas_iuodchunks = asoc->stats.iuodchunks;
	sas.sas_idupchunks = asoc->stats.idupchunks;
	sas.sas_opackets = asoc->stats.opackets;
	sas.sas_ipackets = asoc->stats.ipackets;

	/* New high max rto observed, will return 0 if not a single
	 * RTO update took place. obs_rto_ipaddr will be bogus
	 * in such a case
	 */
	sas.sas_maxrto = asoc->stats.max_obs_rto;
	memcpy(&sas.sas_obs_rto_ipaddr, &asoc->stats.obs_rto_ipaddr,
		sizeof(struct sockaddr_storage));

	/* Mark beginning of a new observation period */
	asoc->stats.max_obs_rto = asoc->rto_min;

	/* Allow the struct to grow and fill in as much as possible */
	len = min_t(size_t, len, sizeof(sas));

	if (put_user(len, optlen))
		return -EFAULT;

	SCTP_DEBUG_PRINTK("sctp_getsockopt_assoc_stat(%d): %d\n",
			  len, sas.sas_assoc_id);

	if (copy_to_user(optval, &sas, len))
		return -EFAULT;

	return 0;
}
