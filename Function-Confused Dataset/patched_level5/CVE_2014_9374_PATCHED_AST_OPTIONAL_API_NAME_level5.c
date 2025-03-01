int CVE_2014_9374_PATCHED_AST_OPTIONAL_API_NAME(ast_websocket_read)(struct ast_websocket *session, char **payload, uint64_t *payload_len, enum ast_websocket_opcode *opcode, int *fragmented)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	char buf[MAXIMUM_FRAME_SIZE] = "";
	int fin = 0;
	int mask_present = 0;
	char *mask = NULL, *new_payload = NULL;
	size_t options_len = 0, frame_size = 0;

	*payload = NULL;
	*payload_len = 0;
	*fragmented = 0;

	if (ws_safe_read(session, &buf[0], MIN_WS_HDR_SZ, opcode)) {
		return 0;
	}
	frame_size += MIN_WS_HDR_SZ;

	/* ok, now we have the first 2 bytes, so we know some flags, opcode and payload length (or whether payload length extension will be required) */
	*opcode = buf[0] & 0xf;
	*payload_len = buf[1] & 0x7f;
	if (*opcode == AST_WEBSOCKET_OPCODE_TEXT || *opcode == AST_WEBSOCKET_OPCODE_BINARY || *opcode == AST_WEBSOCKET_OPCODE_CONTINUATION ||
	    *opcode == AST_WEBSOCKET_OPCODE_PING || *opcode == AST_WEBSOCKET_OPCODE_PONG) {
		fin = (buf[0] >> 7) & 1;
		mask_present = (buf[1] >> 7) & 1;

		/* Based on the mask flag and payload length, determine how much more we need to read before start parsing the rest of the header */
		options_len += mask_present ? 4 : 0;
		options_len += (*payload_len == 126) ? 2 : (*payload_len == 127) ? 8 : 0;
		if (options_len) {
			/* read the rest of the header options */
			if (ws_safe_read(session, &buf[frame_size], options_len, opcode)) {
				return 0;
			}
			frame_size += options_len;
		}
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

		if (*payload_len == 126) {
			/* Grab the 2-byte payload length  */
			*payload_len = ntohs(get_unaligned_uint16(&buf[2]));
			mask = &buf[4];
		} else if (*payload_len == 127) {
			/* Grab the 8-byte payload length  */
			*payload_len = ntohl(get_unaligned_uint64(&buf[2]));
			mask = &buf[10];
		} else {
			/* Just set the mask after the small 2-byte header */
			mask = &buf[2];
		}

		/* Now read the rest of the payload */
		*payload = &buf[frame_size]; /* payload will start here, at the end of the options, if any */
		frame_size = frame_size + (*payload_len); /* final frame size is header + optional headers + payload data */
		if (frame_size > MAXIMUM_FRAME_SIZE) {
			ast_log(LOG_WARNING, "Cannot fit huge websocket frame of %zu bytes\n", frame_size);
			/* The frame won't fit :-( */
			ast_websocket_close(session, 1009);
			return -1;
		}

		if (ws_safe_read(session, (*payload), (*payload_len), opcode)) {
			return 0;
		}
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		/* If a mask is present unmask the payload */
		if (mask_present) {
			unsigned int pos;
			for (pos = 0; pos < *payload_len; pos++) {
				(*payload)[pos] ^= mask[pos % 4];
			}
		}

		/* Per the RFC for PING we need to send back an opcode with the application data as received */
		if ((*opcode == AST_WEBSOCKET_OPCODE_PING) && (ast_websocket_write(session, AST_WEBSOCKET_OPCODE_PONG, *payload, *payload_len))) {
			*payload_len = 0;
			ast_websocket_close(session, 1009);
			return 0;
		}

		if (*payload_len) {
			if (!(new_payload = ast_realloc(session->payload, (session->payload_len + *payload_len)))) {
				ast_log(LOG_WARNING, "Failed allocation: %p, %zu, %"PRIu64"\n",
					session->payload, session->payload_len, *payload_len);
				*payload_len = 0;
				ast_websocket_close(session, 1009);
				return 0;
			}
 
			session->payload = new_payload;
			memcpy((session->payload + session->payload_len), (*payload), (*payload_len));
			session->payload_len += *payload_len;
		} else if (!session->payload_len && session->payload) {
			ast_free(session->payload);
			session->payload = NULL;
		}

		if (!fin && session->reconstruct && (session->payload_len < session->reconstruct)) {
			/* If this is not a final message we need to defer returning it until later */
			if (*opcode != AST_WEBSOCKET_OPCODE_CONTINUATION) {
				session->opcode = *opcode;
			}
			*opcode = AST_WEBSOCKET_OPCODE_CONTINUATION;
			*payload_len = 0;
			*payload = NULL;
		} else {
			if (*opcode == AST_WEBSOCKET_OPCODE_CONTINUATION) {
				if (!fin) {
					/* If this was not actually the final message tell the user it is fragmented so they can deal with it accordingly */
					*fragmented = 1;
				} else {
					/* Final frame in multi-frame so push up the actual opcode */
					*opcode = session->opcode;
				}
			}
			*payload_len = session->payload_len;
			*payload = session->payload;
			session->payload_len = 0;
		}
	} else if (*opcode == AST_WEBSOCKET_OPCODE_CLOSE) {
		/* Make the payload available so the user can look at the reason code if they so desire */
		if ((*payload_len) && (new_payload = ast_realloc(session->payload, *payload_len))) {
			if (ws_safe_read(session, &buf[frame_size], (*payload_len), opcode)) {
				return 0;
			}
			session->payload = new_payload;
			memcpy(session->payload, &buf[frame_size], *payload_len);
			*payload = session->payload;
			frame_size += (*payload_len);
		}

		session->closing = 1;
	} else {
		ast_log(LOG_WARNING, "WebSocket unknown opcode %u\n", *opcode);
		/* We received an opcode that we don't understand, the RFC states that 1003 is for a type of data that can't be accepted... opcodes
		 * fit that, I think. */
		ast_websocket_close(session, 1003);
	}

fclose(para_fp);
	return 0;
}