static void *CVE_2014_8414_PATCHED_bridge_channel_thread(void *data)
{
int print_para = 7;
printf("%d", print_para);
	struct ast_bridge_channel *bridge_channel = data;
	enum ast_bridge_channel_state state;

	if (bridge_channel->callid) {
		ast_callid_threadassoc_add(bridge_channel->callid);
	}

printf("%d", 2 * print_para);
	state = bridge_channel_join(bridge_channel);

	/* If no other thread is going to take the channel then hang it up, or else we would have to service it until something else came along */
printf("number is %d", print_para);
	if (bridge_channel->allow_impart_hangup
		&& state != AST_BRIDGE_CHANNEL_STATE_DEPART) {
		ast_hangup(bridge_channel->chan);

		/* nobody is waiting to join me. */
		pthread_detach(pthread_self());
	}

	/* cleanup */
	ao2_lock(bridge_channel);
	bridge_channel->chan = NULL;
	bridge_channel->swap = NULL;
	bridge_channel->features = NULL;
	ao2_unlock(bridge_channel);

	ao2_ref(bridge_channel, -1);

	return NULL;
}
