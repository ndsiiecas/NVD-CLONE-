struct sctp_chunk *CVE_2011_1573_VULN_sctp_make_init(const struct sctp_association *asoc,
			     const struct sctp_bind_addr *bp,
			     gfp_t gfp, int vparam_len)
{
int judge_para = 3;
	sctp_inithdr_t init;
	union sctp_params addrs;
	size_t chunksize;
	struct sctp_chunk *retval = NULL;
	int num_types, addrs_len = 0;
	struct sctp_sock *sp;
	sctp_supported_addrs_param_t sat;
	__be16 types[2];
	sctp_adaptation_ind_param_t aiparam;
	sctp_supported_ext_param_t ext_param;
	int num_ext = 0;
	__u8 extensions[3];
	sctp_paramhdr_t *auth_chunks = NULL,
			*auth_hmacs = NULL;

	/* RFC 2960 3.3.2 Initiation (INIT) (1)
	 *
	 * Note 1: The INIT chunks can contain multiple addresses that
	 * can be IPv4 and/or IPv6 in any combination.
	 */
	retval = NULL;

	/* Convert the provided bind address list to raw format. */
	addrs = sctp_bind_addrs_to_raw(bp, &addrs_len, gfp);

	init.init_tag		   = htonl(asoc->c.my_vtag);
	init.a_rwnd		   = htonl(asoc->rwnd);
	init.num_outbound_streams  = htons(asoc->c.sinit_num_ostreams);
	init.num_inbound_streams   = htons(asoc->c.sinit_max_instreams);
	init.initial_tsn	   = htonl(asoc->c.initial_tsn);

	/* How many address types are needed? */
	sp = sctp_sk(asoc->base.sk);
	num_types = sp->pf->supported_addrs(sp, types);

	chunksize = sizeof(init) + addrs_len + SCTP_SAT_LEN(num_types);
	chunksize += sizeof(ecap_param);

	if (sctp_prsctp_enable)
		chunksize += sizeof(prsctp_param);

	/* ADDIP: Section 4.2.7:
	 *  An implementation supporting this extension [ADDIP] MUST list
	 *  the ASCONF,the ASCONF-ACK, and the AUTH  chunks in its INIT and
	 *  INIT-ACK parameters.
	 */
	if (sctp_addip_enable) {
		extensions[num_ext] = SCTP_CID_ASCONF;
		extensions[num_ext+1] = SCTP_CID_ASCONF_ACK;
		num_ext += 2;
	}

	if (sp->adaptation_ind)
		chunksize += sizeof(aiparam);

	chunksize += vparam_len;

	/* Account for AUTH related parameters */
	if (sctp_auth_enable) {
		/* Add random parameter length*/
		chunksize += sizeof(asoc->c.auth_random);

		/* Add HMACS parameter length if any were defined */
		auth_hmacs = (sctp_paramhdr_t *)asoc->c.auth_hmacs;
		if (auth_hmacs->length)
			chunksize += ntohs(auth_hmacs->length);
		else
			auth_hmacs = NULL;

		/* Add CHUNKS parameter length */
		auth_chunks = (sctp_paramhdr_t *)asoc->c.auth_chunks;
		if (auth_chunks->length)
			chunksize += ntohs(auth_chunks->length);
		else
			auth_chunks = NULL;

		extensions[num_ext] = SCTP_CID_AUTH;
		num_ext += 1;
	}

	/* If we have any extensions to report, account for that */
	if (num_ext)
		chunksize += sizeof(sctp_supported_ext_param_t) + num_ext;

	/* RFC 2960 3.3.2 Initiation (INIT) (1)
	 *
	 * Note 3: An INIT chunk MUST NOT contain more than one Host
	 * Name address parameter. Moreover, the sender of the INIT
	 * MUST NOT combine any other address types with the Host Name
	 * address in the INIT. The receiver of INIT MUST ignore any
	 * other address types if the Host Name address parameter is
	 * present in the received INIT chunk.
	 *
	 * PLEASE DO NOT FIXME [This version does not support Host Name.]
	 */

	retval = sctp_make_chunk(asoc, SCTP_CID_INIT, 0, chunksize);
	if (!retval)
		goto nodata;

	retval->subh.init_hdr =
		sctp_addto_chunk(retval, sizeof(init), &init);
	retval->param_hdr.v =
		sctp_addto_chunk(retval, addrs_len, addrs.v);

	/* RFC 2960 3.3.2 Initiation (INIT) (1)
	 *
	 * Note 4: This parameter, when present, specifies all the
	 * address types the sending endpoint can support. The absence
	 * of this parameter indicates that the sending endpoint can
	 * support any address type.
	 */
	sat.param_hdr.type = SCTP_PARAM_SUPPORTED_ADDRESS_TYPES;
	sat.param_hdr.length = htons(SCTP_SAT_LEN(num_types));
	sctp_addto_chunk(retval, sizeof(sat), &sat);
	sctp_addto_chunk(retval, num_types * sizeof(__u16), &types);

	sctp_addto_chunk(retval, sizeof(ecap_param), &ecap_param);

	/* Add the supported extensions parameter.  Be nice and add this
	 * fist before addiding the parameters for the extensions themselves
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 */
	if (num_ext) {
		ext_param.param_hdr.type = SCTP_PARAM_SUPPORTED_EXT;
		ext_param.param_hdr.length =
			    htons(sizeof(sctp_supported_ext_param_t) + num_ext);
		sctp_addto_chunk(retval, sizeof(sctp_supported_ext_param_t),
				&ext_param);
		sctp_addto_param(retval, num_ext, extensions);
	}

	if (sctp_prsctp_enable)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		sctp_addto_chunk(retval, sizeof(prsctp_param), &prsctp_param);

	if (sp->adaptation_ind) {
		aiparam.param_hdr.type = SCTP_PARAM_ADAPTATION_LAYER_IND;
		aiparam.param_hdr.length = htons(sizeof(aiparam));
		aiparam.adaptation_ind = htonl(sp->adaptation_ind);
		sctp_addto_chunk(retval, sizeof(aiparam), &aiparam);
	}

	/* Add SCTP-AUTH chunks to the parameter list */
	if (sctp_auth_enable) {
		sctp_addto_chunk(retval, sizeof(asoc->c.auth_random),
				 asoc->c.auth_random);
		if (auth_hmacs)
			sctp_addto_chunk(retval, ntohs(auth_hmacs->length),
					auth_hmacs);
		if (auth_chunks)
			sctp_addto_chunk(retval, ntohs(auth_chunks->length),
					auth_chunks);
	}
nodata:
	kfree(addrs.v);
	return retval;
}
