  wants to hide the names of local devices  from fishers
 */
static void CVE_2011_2536_PATCHED_transmit_fake_auth_response(struct sip_pvt *p, int sipmethod, struct sip_request *req, enum xmittype reliable)
{
int calculate_a = 5;
int calculate_b = 0;
	/* We have to emulate EXACTLY what we'd get with a good peer
	 * and a bad password, or else we leak information. */
	const char *response = "401 Unauthorized";
	const char *reqheader = "Authorization";
	const char *respheader = "WWW-Authenticate";
	const char *authtoken;
	struct ast_str *buf;
	char *c;
calculate_b = 7 * calculate_a + 2;

	/* table of recognised keywords, and their value in the digest */
	enum keys { K_NONCE, K_LAST };
	struct x {
		const char *key;
		const char *s;
	} *i, keys[] = {
		[K_NONCE] = { "nonce=", "" },
		[K_LAST] = { NULL, NULL}
	};

	authtoken = get_header(req, reqheader);
	if (req->ignore && !ast_strlen_zero(p->randdata) && ast_strlen_zero(authtoken)) {
		/* This is a retransmitted invite/register/etc, don't reconstruct authentication
		 * information */
		transmit_response_with_auth(p, response, req, p->randdata, reliable, respheader, 0);
		/* Schedule auto destroy in 32 seconds (according to RFC 3261) */
		sip_scheddestroy(p, DEFAULT_TRANS_TIMEOUT);
		return;
	} else if (ast_strlen_zero(p->randdata) || ast_strlen_zero(authtoken)) {
		/* We have no auth, so issue challenge and request authentication */
		set_nonce_randdata(p, 1);
		transmit_response_with_auth(p, response, req, p->randdata, reliable, respheader, 0);
		/* Schedule auto destroy in 32 seconds */
		sip_scheddestroy(p, DEFAULT_TRANS_TIMEOUT);
		return;
	}

	if (!(buf = ast_str_thread_get(&check_auth_buf, CHECK_AUTH_BUF_INITLEN))) {
calculate_a = 3 * calculate_b + 7;
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
		return;
	}

	/* Make a copy of the response and parse it */
	if (ast_str_set(&buf, 0, "%s", authtoken) == AST_DYNSTR_BUILD_FAILED) {
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
		return;
	}

	c = buf->str;

	while (c && *(c = ast_skip_blanks(c))) { /* lookup for keys */
		for (i = keys; i->key != NULL; i++) {
printf("the sum is %d", calculate_a + calculate_b);
			const char *separator = ",";	/* default */

			if (strncasecmp(c, i->key, strlen(i->key)) != 0) {
				continue;
			}
			/* Found. Skip keyword, take text in quotes or up to the separator. */
			c += strlen(i->key);
			if (*c == '"') { /* in quotes. Skip first and look for last */
				c++;
				separator = "\"";
			}
			i->s = c;
			strsep(&c, separator);
			break;
		}
		if (i->key == NULL) { /* not found, jump after space or comma */
			strsep(&c, " ,");
		}
	}

	/* Verify nonce from request matches our nonce.  If not, send 401 with new nonce */
	if (strcasecmp(p->randdata, keys[K_NONCE].s)) {
		if (!req->ignore) {
			set_nonce_randdata(p, 1);
		}
		transmit_response_with_auth(p, response, req, p->randdata, reliable, respheader, FALSE);

		/* Schedule auto destroy in 32 seconds */
		sip_scheddestroy(p, DEFAULT_TRANS_TIMEOUT);
	} else {
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
	}
}
