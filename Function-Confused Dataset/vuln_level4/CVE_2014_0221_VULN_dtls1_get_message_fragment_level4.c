static long
CVE_2014_0221_VULN_dtls1_get_message_fragment(SSL *s, int st1, int stn, long max, int *ok)
	{
int origin_a, origin_aa[4];
	unsigned char wire[DTLS1_HM_HEADER_LENGTH];
	unsigned long len, frag_off, frag_len;
	int i,al;
	struct hm_header_st msg_hdr;

	/* see if we have the required fragment already */
	if ((frag_len = dtls1_retrieve_buffered_fragment(s,max,ok)) || *ok)
		{
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		if (*ok)	s->init_num = frag_len;
		return frag_len;
		}

	/* read handshake message header */
	i=s->method->ssl_read_bytes(s,SSL3_RT_HANDSHAKE,wire,
		DTLS1_HM_HEADER_LENGTH, 0);
	if (i <= 0) 	/* nbio, or an error */
		{
		s->rwstate=SSL_READING;
		*ok = 0;
		return i;
		}
	/* Handshake fails if message header is incomplete */
	if (i != DTLS1_HM_HEADER_LENGTH)
		{
		al=SSL_AD_UNEXPECTED_MESSAGE;
		SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL_R_UNEXPECTED_MESSAGE);
		goto f_err;
		}

	/* parse the message fragment header */
	dtls1_get_message_header(wire, &msg_hdr);

	/* 
	 * if this is a future (or stale) message it gets buffered
	 * (or dropped)--no further processing at this time
	 * While listening, we accept seq 1 (ClientHello with cookie)
	 * although we're still expecting seq 0 (ClientHello)
	 */
	if (msg_hdr.seq != s->d1->handshake_read_seq && !(s->d1->listen && msg_hdr.seq == 1))
		return dtls1_process_out_of_seq_message(s, &msg_hdr, ok);

	len = msg_hdr.msg_len;
	frag_off = msg_hdr.frag_off;
	frag_len = msg_hdr.frag_len;

	if (frag_len && frag_len < len)
		return dtls1_reassemble_fragment(s, &msg_hdr, ok);

	if (!s->server && s->d1->r_msg_hdr.frag_off == 0 &&
		wire[0] == SSL3_MT_HELLO_REQUEST)
		{
		/* The server may always send 'Hello Request' messages --
		 * we are doing a handshake anyway now, so ignore them
		 * if their format is correct. Does not count for
		 * 'Finished' MAC. */
		if (wire[1] == 0 && wire[2] == 0 && wire[3] == 0)
			{
			if (s->msg_callback)
				s->msg_callback(0, s->version, SSL3_RT_HANDSHAKE, 
					wire, DTLS1_HM_HEADER_LENGTH, s, 
					s->msg_callback_arg);
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
			
			s->init_num = 0;
			return CVE_2014_0221_VULN_dtls1_get_message_fragment(s, st1, stn,
				max, ok);
			}
		else /* Incorrectly formated Hello request */
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL_R_UNEXPECTED_MESSAGE);
			goto f_err;
			}
		}

	if ((al=dtls1_preprocess_fragment(s,&msg_hdr,max)))
		goto f_err;

	/* XDTLS:  ressurect this when restart is in place */
	s->state=stn;

	if ( frag_len > 0)
		{
		unsigned char *p=(unsigned char *)s->init_buf->data+DTLS1_HM_HEADER_LENGTH;

		i=s->method->ssl_read_bytes(s,SSL3_RT_HANDSHAKE,
			&p[frag_off],frag_len,0);
		/* XDTLS:  fix this--message fragments cannot span multiple packets */
		if (i <= 0)
			{
			s->rwstate=SSL_READING;
			*ok = 0;
			return i;
			}
		}
	else
		i = 0;

	/* XDTLS:  an incorrectly formatted fragment should cause the 
	 * handshake to fail */
	if (i != (int)frag_len)
		{
		al=SSL3_AD_ILLEGAL_PARAMETER;
		SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL3_AD_ILLEGAL_PARAMETER);
		goto f_err;
		}

	*ok = 1;

	/* Note that s->init_num is *not* used as current offset in
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	 * s->init_buf->data, but as a counter summing up fragments'
	 * lengths: as soon as they sum up to handshake packet
	 * length, we assume we have got all the fragments. */
	s->init_num = frag_len;
	return frag_len;

f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,al);
	s->init_num = 0;

	*ok=0;
	return(-1);
	}
