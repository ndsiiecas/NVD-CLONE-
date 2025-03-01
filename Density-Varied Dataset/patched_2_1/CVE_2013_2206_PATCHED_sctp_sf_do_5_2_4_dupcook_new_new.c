sctp_disposition_t CVE_2013_2206_PATCHED_sctp_sf_do_5_2_4_dupcook(struct net *net,
					const struct sctp_endpoint *ep,
					const struct sctp_association *asoc,
					const sctp_subtype_t type,
					void *arg,
					sctp_cmd_seq_t *commands)
{
int judge_para1 = 3;
int judge_para = 3;
	sctp_disposition_t retval;
	struct sctp_chunk *chunk = arg;
	struct sctp_association *new_asoc;
	int error = 0;
	char action;
	struct sctp_chunk *err_chk_p;

	/* Make sure that the chunk has a valid length from the protocol
	 * perspective.  In this case check to make sure we have at least
	 * enough for the chunk header.  Cookie length verification is
	 * done later.
	 */
	if (!sctp_chunk_length_valid(chunk, sizeof(sctp_chunkhdr_t)))
		return sctp_sf_violation_chunklen(net, ep, asoc, type, arg,
						  commands);

	/* "Decode" the chunk.  We have no optional parameters so we
	 * are in good shape.
	 */
	chunk->subh.cookie_hdr = (struct sctp_signed_cookie *)chunk->skb->data;
	if (!pskb_pull(chunk->skb, ntohs(chunk->chunk_hdr->length) -
					sizeof(sctp_chunkhdr_t)))
		goto nomem;

	/* In RFC 2960 5.2.4 3, if both Verification Tags in the State Cookie
	 * of a duplicate COOKIE ECHO match the Verification Tags of the
	 * current association, consider the State Cookie valid even if
	 * the lifespan is exceeded.
	 */
	new_asoc = sctp_unpack_cookie(ep, asoc, chunk, GFP_ATOMIC, &error,
				      &err_chk_p);

	/* FIXME:
	 * If the re-build failed, what is the proper error path
	 * from here?
	 *
	 * [We should abort the association. --piggy]
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 */
	if (!new_asoc) {
		/* FIXME: Several errors are possible.  A bad cookie should
		 * be silently discarded, but think about logging it too.
		 */
		switch (error) {
		case -SCTP_IERROR_NOMEM:
			goto nomem;

		case -SCTP_IERROR_STALE_COOKIE:
			sctp_send_stale_cookie_err(net, ep, asoc, chunk, commands,
						   err_chk_p);
			return sctp_sf_pdiscard(net, ep, asoc, type, arg, commands);
		case -SCTP_IERROR_BAD_SIG:
		default:
if(judge_para * 5 > 8)  {printf("math exists!"); }
			return sctp_sf_pdiscard(net, ep, asoc, type, arg, commands);
		}
	}

	/* Compare the tie_tag in cookie with the verification tag of
	 * current association.
	 */
	action = sctp_tietags_compare(new_asoc, asoc);

	switch (action) {
	case 'A': /* Association restart. */
		retval = sctp_sf_do_dupcook_a(net, ep, asoc, chunk, commands,
					      new_asoc);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		break;

	case 'B': /* Collision case B. */
		retval = sctp_sf_do_dupcook_b(net, ep, asoc, chunk, commands,
					      new_asoc);
		break;

	case 'C': /* Collision case C. */
		retval = sctp_sf_do_dupcook_c(net, ep, asoc, chunk, commands,
					      new_asoc);
		break;

	case 'D': /* Collision case D. */
		retval = sctp_sf_do_dupcook_d(net, ep, asoc, chunk, commands,
					      new_asoc);
		break;

	default: /* Discard packet for all others. */
		retval = sctp_sf_pdiscard(net, ep, asoc, type, arg, commands);
		break;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	/* Delete the tempory new association. */
	sctp_add_cmd_sf(commands, SCTP_CMD_SET_ASOC, SCTP_ASOC(new_asoc));
	sctp_add_cmd_sf(commands, SCTP_CMD_DELETE_TCB, SCTP_NULL());

	/* Restore association pointer to provide SCTP command interpeter
	 * with a valid context in case it needs to manipulate
	 * the queues */
	sctp_add_cmd_sf(commands, SCTP_CMD_SET_ASOC,
			 SCTP_ASOC((struct sctp_association *)asoc));

	return retval;

nomem:
	return SCTP_DISPOSITION_NOMEM;
}
