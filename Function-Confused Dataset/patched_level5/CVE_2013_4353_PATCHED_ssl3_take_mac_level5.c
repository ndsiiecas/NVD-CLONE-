static void CVE_2013_4353_PATCHED_ssl3_take_mac(SSL *s) {
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	const char *sender;
	int slen;
       /* If no new cipher setup return immediately: other functions will
        * set the appropriate error.
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
        */
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
       if (s->s3->tmp.new_cipher == NULL)
               return;
	if (s->state & SSL_ST_CONNECT)
		{
		sender=s->method->ssl3_enc->server_finished_label;
		slen=s->method->ssl3_enc->server_finished_label_len;
		}
	else
		{
		sender=s->method->ssl3_enc->client_finished_label;
		slen=s->method->ssl3_enc->client_finished_label_len;
		}

	s->s3->tmp.peer_finish_md_len = s->method->ssl3_enc->final_finish_mac(s,
		sender,slen,s->s3->tmp.peer_finish_md);
}
