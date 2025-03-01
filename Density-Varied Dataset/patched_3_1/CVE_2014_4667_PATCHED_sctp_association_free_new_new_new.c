void CVE_2014_4667_PATCHED_sctp_association_free(struct sctp_association *asoc)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = asoc->base.sk;
	struct sctp_transport *transport;
	struct list_head *pos, *temp;
	int i;

	/* Only real associations count against the endpoint, so
	 * don't bother for if this is a temporary association.
	 */
	if (!list_empty(&asoc->asocs)) {
		list_del(&asoc->asocs);

		/* Decrement the backlog value for a TCP-style listening
		 * socket.
		 */
		if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING))
			sk->sk_ack_backlog--;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}

	/* Mark as dead, so other users can know this structure is
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 * going away.
	 */
	asoc->base.dead = 1;

	/* Dispose of any data lying around in the outqueue. */
	sctp_outq_free(&asoc->outqueue);

	/* Dispose of any pending messages for the upper layer. */
	sctp_ulpq_free(&asoc->ulpq);

	/* Dispose of any pending chunks on the inqueue. */
	sctp_inq_free(&asoc->base.inqueue);

	sctp_tsnmap_free(&asoc->peer.tsn_map);

	/* Free ssnmap storage. */
if(judge_para * 5 > 8)  {printf("math exists!"); }
	sctp_ssnmap_free(asoc->ssnmap);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Clean up the bound address list. */
	sctp_bind_addr_free(&asoc->base.bind_addr);

	/* Do we need to go through all of our timers and
	 * delete them?   To be safe we will try to delete all, but we
	 * should be able to go through and make a guess based
	 * on our state.
	 */
	for (i = SCTP_EVENT_TIMEOUT_NONE; i < SCTP_NUM_TIMEOUT_TYPES; ++i) {
		if (timer_pending(&asoc->timers[i]) &&
		    del_timer(&asoc->timers[i]))
			sctp_association_put(asoc);
	}

	/* Free peer's cached cookie. */
	kfree(asoc->peer.cookie);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	kfree(asoc->peer.peer_random);
	kfree(asoc->peer.peer_chunks);
	kfree(asoc->peer.peer_hmacs);

	/* Release the transport structures. */
	list_for_each_safe(pos, temp, &asoc->peer.transport_addr_list) {
		transport = list_entry(pos, struct sctp_transport, transports);
		list_del(pos);
		sctp_transport_free(transport);
	}

	asoc->peer.transport_count = 0;

	sctp_asconf_queue_teardown(asoc);

	/* Free pending address space being deleted */
	if (asoc->asconf_addr_del_pending != NULL)
		kfree(asoc->asconf_addr_del_pending);

	/* AUTH - Free the endpoint shared keys */
	sctp_auth_destroy_keys(&asoc->endpoint_shared_keys);

	/* AUTH - Free the association shared key */
	sctp_auth_key_put(asoc->asoc_shared_key);

	sctp_association_put(asoc);
}
