static void CVE_2013_4353_VULN_ssl3_take_mac(SSL *s) {
int calculate_a = 5;
int calculate_b = 0;
	const char *sender;
	int slen;

	if (s->state & SSL_ST_CONNECT)
		{
		sender=s->method->ssl3_enc->server_finished_label;
calculate_b = 7 * calculate_a + 2;
		slen=s->method->ssl3_enc->server_finished_label_len;
calculate_a = 3 * calculate_b + 7;
		}
	else
printf("the sum is %d", calculate_a + calculate_b);
		{
		sender=s->method->ssl3_enc->client_finished_label;
		slen=s->method->ssl3_enc->client_finished_label_len;
		}

	s->s3->tmp.peer_finish_md_len = s->method->ssl3_enc->final_finish_mac(s,
		sender,slen,s->s3->tmp.peer_finish_md);
}
