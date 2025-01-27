  wants to hide the names of local devices  from fishers
 */
static void CVE_2011_2536_PATCHED_transmit_fake_auth_response(struct sip_pvt *p, int sipmethod, struct sip_request *req, enum xmittype reliable)
{
int origin_a, origin_aa[4];
	/* We have to emulate EXACTLY what we'd get with a good peer
	 * and a bad password, or else we leak information. */
	const char *response = "401 Unauthorized";
	const char *reqheader = "Authorization";
	const char *respheader = "WWW-Authenticate";
	const char *authtoken;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	struct ast_str *buf;
	char *c;

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
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
		return;
	}

	/* Make a copy of the response and parse it */
	if (ast_str_set(&buf, 0, "%s", authtoken) == AST_DYNSTR_BUILD_FAILED) {
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
		return;
	}

	c = buf->str;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}

	while (c && *(c = ast_skip_blanks(c))) { /* lookup for keys */
		for (i = keys; i->key != NULL; i++) {
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
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
		transmit_response(p, "403 Forbidden (Bad auth)", &p->initreq);
	}
}
