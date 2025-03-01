static void CVE_2006_6106_PATCHED_cmtp_recv_interopmsg(struct cmtp_session *session, struct sk_buff *skb)
{
int origin_a, origin_aa[4];
	struct capi_ctr *ctrl = &session->ctrl;
	struct cmtp_application *application;
	__u16 appl, msgnum, func, info;
	__u32 controller;

	BT_DBG("session %p skb %p len %d", session, skb, skb->len);

	switch (CAPIMSG_SUBCOMMAND(skb->data)) {
	case CAPI_CONF:
		if (skb->len < CAPI_MSG_BASELEN + 10)
			break;

		func = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 5);
		info = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 8);

		switch (func) {
		case CAPI_FUNCTION_REGISTER:
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
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
			if (skb->len < CAPI_MSG_BASELEN + 11 + sizeof(capi_profile))
				break;

			controller = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 11);
			msgnum = CAPIMSG_MSGID(skb->data);

			if (!info && (msgnum == CMTP_INITIAL_MSGNUM)) {
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
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
			if (skb->len < CAPI_MSG_BASELEN + 15)
				break;

			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 10);

			if (!info && ctrl) {
				int len = min_t(uint, CAPI_MANUFACTURER_LEN,
						skb->data[CAPI_MSG_BASELEN + 14]);

				memset(ctrl->manu, 0, CAPI_MANUFACTURER_LEN);
				strncpy(ctrl->manu,
					skb->data + CAPI_MSG_BASELEN + 15, len);
			}

			break;

		case CAPI_FUNCTION_GET_VERSION:
			if (skb->len < CAPI_MSG_BASELEN + 32)
				break;

			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 12);

			if (!info && ctrl) {
				ctrl->version.majorversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 16);
				ctrl->version.minorversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 20);
				ctrl->version.majormanuversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 24);
				ctrl->version.minormanuversion = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 28);
			}

			break;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}

		case CAPI_FUNCTION_GET_SERIAL_NUMBER:
			if (skb->len < CAPI_MSG_BASELEN + 17)
				break;

			controller = CAPIMSG_U32(skb->data, CAPI_MSG_BASELEN + 12);

			if (!info && ctrl) {
				int len = min_t(uint, CAPI_SERIAL_LEN,
						skb->data[CAPI_MSG_BASELEN + 16]);

				memset(ctrl->serial, 0, CAPI_SERIAL_LEN);
				strncpy(ctrl->serial,
					skb->data + CAPI_MSG_BASELEN + 17, len);
			}

			break;
		}

		break;

	case CAPI_IND:
		if (skb->len < CAPI_MSG_BASELEN + 6)
			break;

		func = CAPIMSG_U16(skb->data, CAPI_MSG_BASELEN + 3);

		if (func == CAPI_FUNCTION_LOOPBACK) {
			int len = min_t(uint, skb->len - CAPI_MSG_BASELEN - 6,
						skb->data[CAPI_MSG_BASELEN + 5]);
			appl = CAPIMSG_APPID(skb->data);
			msgnum = CAPIMSG_MSGID(skb->data);
			cmtp_send_interopmsg(session, CAPI_RESP, appl, msgnum, func,
						skb->data + CAPI_MSG_BASELEN + 6, len);
		}

		break;
	}

	kfree_skb(skb);
}
