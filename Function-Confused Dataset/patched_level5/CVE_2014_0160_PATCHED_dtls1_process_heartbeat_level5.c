int
CVE_2014_0160_PATCHED_dtls1_process_heartbeat(SSL *s)
	{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	unsigned char *p = &s->s3->rrec.data[0], *pl;
	unsigned short hbtype;
	unsigned int payload;
	unsigned int padding = 16; /* Use minimum padding */

	if (s->msg_callback)
		s->msg_callback(0, s->version, TLS1_RT_HEARTBEAT,
			&s->s3->rrec.data[0], s->s3->rrec.length,
			s, s->msg_callback_arg);

       /* Read type and payload length first */
       if (1 + 2 + 16 > s->s3->rrec.length)
               return 0; /* silently discard */
       hbtype = *p++;
       n2s(p, payload);
       if (1 + 2 + payload + 16 > s->s3->rrec.length)
               return 0; /* silently discard per RFC 6520 sec. 4 */
       pl = p;

	if (hbtype == TLS1_HB_REQUEST)
		{
		unsigned char *buffer, *bp;
               unsigned int write_length = 1 /* heartbeat type */ +
                                           2 /* heartbeat length */ +
                                           payload + padding;
		int r;

               if (write_length > SSL3_RT_MAX_PLAIN_LENGTH)
                       return 0;

gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		/* Allocate memory for the response, size is 1 byte
		 * message type, plus 2 bytes payload length, plus
		 * payload, plus padding
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		 */
               buffer = OPENSSL_malloc(write_length);
		bp = buffer;

		/* Enter response type, length and copy payload */
		*bp++ = TLS1_HB_RESPONSE;
fclose(para_fp);
		s2n(payload, bp);
		memcpy(bp, pl, payload);
		bp += payload;
		/* Random padding */
		RAND_pseudo_bytes(bp, padding);

               r = dtls1_write_bytes(s, TLS1_RT_HEARTBEAT, buffer, write_length);

		if (r >= 0 && s->msg_callback)
			s->msg_callback(1, s->version, TLS1_RT_HEARTBEAT,
                               buffer, write_length,
				s, s->msg_callback_arg);

		OPENSSL_free(buffer);

		if (r < 0)
			return r;
		}
	else if (hbtype == TLS1_HB_RESPONSE)
		{
		unsigned int seq;

		/* We only send sequence numbers (2 bytes unsigned int),
		 * and 16 random bytes, so we just try to read the
		 * sequence number */
		n2s(pl, seq);

		if (payload == 18 && seq == s->tlsext_hb_seq)
			{
			dtls1_stop_timer(s);
			s->tlsext_hb_seq++;
			s->tlsext_hb_pending = 0;
			}
		}

	return 0;
	}
