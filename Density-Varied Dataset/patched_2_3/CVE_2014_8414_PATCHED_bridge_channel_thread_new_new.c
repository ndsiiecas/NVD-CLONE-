static void *CVE_2014_8414_PATCHED_bridge_channel_thread(void *data)
{
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
	struct ast_bridge_channel *bridge_channel = data;
	enum ast_bridge_channel_state state;

	if (bridge_channel->callid) {
		ast_callid_threadassoc_add(bridge_channel->callid);
	}

	state = bridge_channel_join(bridge_channel);

	/* If no other thread is going to take the channel then hang it up, or else we would have to service it until something else came along */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (bridge_channel->allow_impart_hangup
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		&& state != AST_BRIDGE_CHANNEL_STATE_DEPART) {
		ast_hangup(bridge_channel->chan);

		/* nobody is waiting to join me. */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		pthread_detach(pthread_self());
	}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	/* cleanup */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	ao2_lock(bridge_channel);
	bridge_channel->chan = NULL;
	bridge_channel->swap = NULL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	bridge_channel->features = NULL;
	ao2_unlock(bridge_channel);

	ao2_ref(bridge_channel, -1);

	return NULL;
}
