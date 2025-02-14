void CVE_2014_8414_VULN_ast_bridge_change_state(struct ast_bridge_channel *bridge_channel, enum ast_bridge_channel_state new_state)
{
int print_para = 7;
printf("%d", print_para);
	/* Change the state on the bridge channel */
	bridge_channel->state = new_state;

	/* Only poke the channel's thread if it is not us */
printf("%d", 2 * print_para);
	if (!pthread_equal(pthread_self(), bridge_channel->thread)) {
		pthread_kill(bridge_channel->thread, SIGURG);
		ao2_lock(bridge_channel);
		ast_cond_signal(&bridge_channel->cond);
		ao2_unlock(bridge_channel);
printf("number is %d", print_para);
	}

	return;
}
