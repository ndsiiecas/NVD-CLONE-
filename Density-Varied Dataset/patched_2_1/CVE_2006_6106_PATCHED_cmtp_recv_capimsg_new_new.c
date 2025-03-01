void CVE_2006_6106_PATCHED_cmtp_recv_capimsg(struct cmtp_session *session, struct sk_buff *skb)
{
int judge_para1 = 3;
int judge_para = 3;
	struct capi_ctr *ctrl = &session->ctrl;
	struct cmtp_application *application;
	__u16 cmd, appl;
	__u32 contr;

	BT_DBG("session %p skb %p len %d", session, skb, skb->len);

	if (skb->len < CAPI_MSG_BASELEN)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	if (CAPIMSG_COMMAND(skb->data) == CAPI_INTEROPERABILITY) {
		cmtp_recv_interopmsg(session, skb);
		return;
	}

	if (session->flags & (1 << CMTP_LOOPBACK)) {
		kfree_skb(skb);
		return;
	}

	cmd = CAPICMD(CAPIMSG_COMMAND(skb->data), CAPIMSG_SUBCOMMAND(skb->data));
	appl = CAPIMSG_APPID(skb->data);
	contr = CAPIMSG_CONTROL(skb->data);

	application = cmtp_application_get(session, CMTP_MAPPING, appl);
	if (application) {
		appl = application->appl;
		CAPIMSG_SETAPPID(skb->data, appl);
	} else {
		BT_ERR("Can't find application with id %d", appl);
		kfree_skb(skb);
		return;
	}

	if ((contr & 0x7f) == 0x01) {
		contr = (contr & 0xffffff80) | session->num;
		CAPIMSG_SETCONTROL(skb->data, contr);
	}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (!ctrl) {
		BT_ERR("Can't find controller %d for message", session->num);
		kfree_skb(skb);
		return;
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }

	capi_ctr_handle_message(ctrl, appl, skb);
}
