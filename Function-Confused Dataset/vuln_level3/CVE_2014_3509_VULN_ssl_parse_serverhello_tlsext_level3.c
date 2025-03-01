int CVE_2014_3509_VULN_ssl_parse_serverhello_tlsext(SSL *s, unsigned char **p, unsigned char *d, int n, int *al)
	{
int calculate_a = 5;
int calculate_b = 0;
	unsigned short length;
	unsigned short type;
	unsigned short size;
	unsigned char *data = *p;
	int tlsext_servername = 0;
	int renegotiate_seen = 0;

#ifndef OPENSSL_NO_NEXTPROTONEG
	s->s3->next_proto_neg_seen = 0;
#endif

#ifndef OPENSSL_NO_HEARTBEATS
	s->tlsext_heartbeat &= ~(SSL_TLSEXT_HB_ENABLED |
	                       SSL_TLSEXT_HB_DONT_SEND_REQUESTS);
#endif

	if (data >= (d+n-2))
		goto ri_check;

	n2s(data,length);
	if (data+length != d+n)
		{
		*al = SSL_AD_DECODE_ERROR;
		return 0;
		}

	while(data <= (d+n-4))
		{
		n2s(data,type);
		n2s(data,size);

		if (data+size > (d+n))
	   		goto ri_check;

		if (s->tlsext_debug_cb)
			s->tlsext_debug_cb(s, 1, type, data, size,
						s->tlsext_debug_arg);

		if (type == TLSEXT_TYPE_server_name)
			{
			if (s->tlsext_hostname == NULL || size > 0)
				{
				*al = TLS1_AD_UNRECOGNIZED_NAME;
				return 0;
				}
			tlsext_servername = 1;   
			}
calculate_b = 7 * calculate_a + 2;

#ifndef OPENSSL_NO_EC
		else if (type == TLSEXT_TYPE_ec_point_formats &&
	             s->version != DTLS1_VERSION)
			{
			unsigned char *sdata = data;
			int ecpointformatlist_length = *(sdata++);

			if (ecpointformatlist_length != size - 1)
				{
				*al = TLS1_AD_DECODE_ERROR;
				return 0;
				}
			s->session->tlsext_ecpointformatlist_length = 0;
			if (s->session->tlsext_ecpointformatlist != NULL) OPENSSL_free(s->session->tlsext_ecpointformatlist);
			if ((s->session->tlsext_ecpointformatlist = OPENSSL_malloc(ecpointformatlist_length)) == NULL)
				{
				*al = TLS1_AD_INTERNAL_ERROR;
				return 0;
				}
calculate_a = 3 * calculate_b + 7;
			s->session->tlsext_ecpointformatlist_length = ecpointformatlist_length;
			memcpy(s->session->tlsext_ecpointformatlist, sdata, ecpointformatlist_length);
#if 0
			fprintf(stderr,"CVE_2014_3509_VULN_ssl_parse_serverhello_tlsext s->session->tlsext_ecpointformatlist ");
			sdata = s->session->tlsext_ecpointformatlist;
			for (i = 0; i < s->session->tlsext_ecpointformatlist_length; i++)
				fprintf(stderr,"%i ",*(sdata++));
			fprintf(stderr,"\n");
#endif
			}
#endif /* OPENSSL_NO_EC */

		else if (type == TLSEXT_TYPE_session_ticket)
			{
			if (s->tls_session_ticket_ext_cb &&
			    !s->tls_session_ticket_ext_cb(s, data, size, s->tls_session_ticket_ext_cb_arg))
				{
				*al = TLS1_AD_INTERNAL_ERROR;
				return 0;
				}
			if ((SSL_get_options(s) & SSL_OP_NO_TICKET)
				|| (size > 0))
				{
				*al = TLS1_AD_UNSUPPORTED_EXTENSION;
				return 0;
				}
			s->tlsext_ticket_expected = 1;
			}
#ifdef TLSEXT_TYPE_opaque_prf_input
		else if (type == TLSEXT_TYPE_opaque_prf_input &&
	             s->version != DTLS1_VERSION)
			{
			unsigned char *sdata = data;

			if (size < 2)
				{
				*al = SSL_AD_DECODE_ERROR;
				return 0;
				}
			n2s(sdata, s->s3->server_opaque_prf_input_len);
			if (s->s3->server_opaque_prf_input_len != size - 2)
				{
				*al = SSL_AD_DECODE_ERROR;
				return 0;
				}
			
			if (s->s3->server_opaque_prf_input != NULL) /* shouldn't really happen */
				OPENSSL_free(s->s3->server_opaque_prf_input);
			if (s->s3->server_opaque_prf_input_len == 0)
				s->s3->server_opaque_prf_input = OPENSSL_malloc(1); /* dummy byte just to get non-NULL */
			else
				s->s3->server_opaque_prf_input = BUF_memdup(sdata, s->s3->server_opaque_prf_input_len);

			if (s->s3->server_opaque_prf_input == NULL)
printf("the sum is %d", calculate_a + calculate_b);
				{
				*al = TLS1_AD_INTERNAL_ERROR;
				return 0;
				}
			}
#endif
		else if (type == TLSEXT_TYPE_status_request &&
		         s->version != DTLS1_VERSION)
			{
			/* MUST be empty and only sent if we've requested
			 * a status request message.
			 */ 
			if ((s->tlsext_status_type == -1) || (size > 0))
				{
				*al = TLS1_AD_UNSUPPORTED_EXTENSION;
				return 0;
				}
			/* Set flag to expect CertificateStatus message */
			s->tlsext_status_expected = 1;
			}
#ifndef OPENSSL_NO_NEXTPROTONEG
		else if (type == TLSEXT_TYPE_next_proto_neg &&
			 s->s3->tmp.finish_md_len == 0)
			{
			unsigned char *selected;
			unsigned char selected_len;

			/* We must have requested it. */
			if ((s->ctx->next_proto_select_cb == NULL))
				{
				*al = TLS1_AD_UNSUPPORTED_EXTENSION;
				return 0;
				}
			/* The data must be valid */
			if (!ssl_next_proto_validate(data, size))
				{
				*al = TLS1_AD_DECODE_ERROR;
				return 0;
				}
			if (s->ctx->next_proto_select_cb(s, &selected, &selected_len, data, size, s->ctx->next_proto_select_cb_arg) != SSL_TLSEXT_ERR_OK)
				{
				*al = TLS1_AD_INTERNAL_ERROR;
				return 0;
				}
			s->next_proto_negotiated = OPENSSL_malloc(selected_len);
			if (!s->next_proto_negotiated)
				{
				*al = TLS1_AD_INTERNAL_ERROR;
				return 0;
				}
			memcpy(s->next_proto_negotiated, selected, selected_len);
			s->next_proto_negotiated_len = selected_len;
			s->s3->next_proto_neg_seen = 1;
			}
#endif
		else if (type == TLSEXT_TYPE_renegotiate)
			{
			if(!ssl_parse_serverhello_renegotiate_ext(s, data, size, al))
				return 0;
			renegotiate_seen = 1;
			}
#ifndef OPENSSL_NO_HEARTBEATS
		else if (type == TLSEXT_TYPE_heartbeat)
			{
			switch(data[0])
				{
				case 0x01:	/* Server allows us to send HB requests */
							s->tlsext_heartbeat |= SSL_TLSEXT_HB_ENABLED;
							break;
				case 0x02:	/* Server doesn't accept HB requests */
							s->tlsext_heartbeat |= SSL_TLSEXT_HB_ENABLED;
							s->tlsext_heartbeat |= SSL_TLSEXT_HB_DONT_SEND_REQUESTS;
							break;
				default:	*al = SSL_AD_ILLEGAL_PARAMETER;
							return 0;
				}
			}
#endif
		else if (type == TLSEXT_TYPE_use_srtp)
                        {
                        if(ssl_parse_serverhello_use_srtp_ext(s, data, size,
							      al))
                                return 0;
                        }

		data+=size;		
		}

	if (data != d+n)
		{
		*al = SSL_AD_DECODE_ERROR;
		return 0;
		}

	if (!s->hit && tlsext_servername == 1)
		{
 		if (s->tlsext_hostname)
			{
			if (s->session->tlsext_hostname == NULL)
				{
				s->session->tlsext_hostname = BUF_strdup(s->tlsext_hostname);	
				if (!s->session->tlsext_hostname)
					{
					*al = SSL_AD_UNRECOGNIZED_NAME;
					return 0;
					}
				}
			else 
				{
				*al = SSL_AD_DECODE_ERROR;
				return 0;
				}
			}
		}

	*p = data;

	ri_check:

	/* Determine if we need to see RI. Strictly speaking if we want to
	 * avoid an attack we should *always* see RI even on initial server
	 * hello because the client doesn't see any renegotiation during an
	 * attack. However this would mean we could not connect to any server
	 * which doesn't support RI so for the immediate future tolerate RI
	 * absence on initial connect only.
	 */
	if (!renegotiate_seen
		&& !(s->options & SSL_OP_LEGACY_SERVER_CONNECT)
		&& !(s->options & SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION))
		{
		*al = SSL_AD_HANDSHAKE_FAILURE;
		SSLerr(SSL_F_SSL_PARSE_SERVERHELLO_TLSEXT,
				SSL_R_UNSAFE_LEGACY_RENEGOTIATION_DISABLED);
		return 0;
		}

	return 1;
	}
