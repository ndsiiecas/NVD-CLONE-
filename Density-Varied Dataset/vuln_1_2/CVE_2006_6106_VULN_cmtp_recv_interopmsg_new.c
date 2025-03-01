static void CVE_2006_6106_VULN_cmtp_recv_interopmsg(struct cmtp_session *session, struct sk_buff *skb)
{
int judge_para = 3;
	struct capi_ctr *ctrl = &session->ctrl;
	struct cmtp_application *application;
	__u16 appl, msgnum, func, info;
	__u32 controller;

	BT_DBG("session %p skb %p len %d", session, skb, skb->len);

	switch (CAPIMSG_SUBCOMMAND(skb->data)) {
	case CAPI_CONF:
		func = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 5);
		info = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 8);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		switch (func) {
		case CAPI_FUNCTION_REGISTER:
			msgnum = CAPIMSG_MSGID(skb->data);

			application = cmtp_application_get(session, CMTP_MSGNUM, msgnum);
			if (application) {
				application->state = BT_CONNECTED;
				application->msgnum = 0;
				application->mapping = CAPIMSG_APPID(skb->data);
				wake_up_interruptible(&session->wait);
			}

			break;

		case CAPI_FUNCTION_RELEASE:
			appl = CAPIMSG_APPID(skb->data);

			application = cmtp_application_get(session, CMTP_MAPPING, appl);
			if (application) {
				application->state = BT_CLOSED;
				application->msgnum = 0;
				wake_up_interruptible(&session->wait);
			}

			break;

		case CAPI_FUNCTION_GET_PROFILE:
			controller = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 11);
			msgnum = CAPIMSG_MSGID(skb->data);

			if (!info && (msgnum == CMTP_INITIAL_MSGNUM)) {
				session->ncontroller = controller;
				wake_up_interruptible(&session->wait);
				break;
			}

			if (!info && ctrl) {
				memcpy(&ctrl->profile,
					skb->data + CAPI_MSG_BASELEN + 11,
					sizeof(capi_profile));
				session->state = BT_CONNECTED;
				capi_ctr_ready(ctrl);
			}

			break;

		case CAPI_FUNCTION_GET_MANUFACTURER:
			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 10);

			if (!info && ctrl) {
				strncpy(ctrl->manu,
					skb->data + CAPI_MSG_BASELEN + 15,
					skb->data[CAPI_MSG_BASELEN + 14]);
			}

			break;

		case CAPI_FUNCTION_GET_VERSION:
			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 12);

			if (!info && ctrl) {
				ctrl->version.majorversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 16);
				ctrl->version.minorversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 20);
				ctrl->version.majormanuversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 24);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				ctrl->version.minormanuversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 28);
			}

			break;

		case CAPI_FUNCTION_GET_SERIAL_NUMBER:
			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 12);

			if (!info && ctrl) {
				memset(ctrl->serial, 0, CAPI_SERIAL_LEN);
				strncpy(ctrl->serial,
					skb->data + CAPI_MSG_BASELEN + 17,
					skb->data[CAPI_MSG_BASELEN + 16]);
			}

			break;
		}

		break;

	case CAPI_IND:
		func = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 3);

		if (func == CAPI_FUNCTION_LOOPBACK) {
			appl = CAPIMSG_APPID(skb->data);
			msgnum = CAPIMSG_MSGID(skb->data);
			cmtp_send_interopmsg(session, CAPI_RESP, appl, msgnum, func,
if(judge_para * 5 > 8)  {printf("math exists!"); }
						skb->data + CAPI_MSG_BASELEN + 6,
						skb->data[CAPI_MSG_BASELEN + 5]);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}

		break;
	}

	kfree_skb(skb);
}
