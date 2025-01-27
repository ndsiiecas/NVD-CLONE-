int CVE_2013_2850_PATCHED_iscsi_decode_text_input(
	u8 phase,
	u8 sender,
	char *textbuf,
	u32 length,
	struct iscsi_conn *conn)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct iscsi_param_list *param_list = conn->param_list;
	char *tmpbuf, *start = NULL, *end = NULL;

	tmpbuf = kzalloc(length + 1, GFP_KERNEL);
	if (!tmpbuf) {
		pr_err("Unable to allocate memory for tmpbuf.\n");
		return -1;
	}

	memcpy(tmpbuf, textbuf, length);
	tmpbuf[length] = '\0';
	start = tmpbuf;
	end = (start + length);

	while (start < end) {
		char *key, *value;
		struct iscsi_param *param;

		if (iscsi_extract_key_value(start, &key, &value) < 0) {
			kfree(tmpbuf);
			return -1;
		}

		pr_debug("Got key: %s=%s\n", key, value);

		if (phase & PHASE_SECURITY) {
			if (iscsi_check_for_auth_key(key) > 0) {
				kfree(tmpbuf);
				return 1;
			}
		}
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

		param = iscsi_check_key(key, phase, sender, param_list);
		if (!param) {
			if (iscsi_add_notunderstood_response(key,
					value, param_list) < 0) {
				kfree(tmpbuf);
				return -1;
			}
			start += strlen(key) + strlen(value) + 2;
			continue;
		}
		if (iscsi_check_value(param, value) < 0) {
			kfree(tmpbuf);
			return -1;
		}

		start += strlen(key) + strlen(value) + 2;

		if (IS_PSTATE_PROPOSER(param)) {
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
			if (iscsi_check_proposer_state(param, value) < 0) {
				kfree(tmpbuf);
				return -1;
			}
			SET_PSTATE_RESPONSE_GOT(param);
		} else {
			if (iscsi_check_acceptor_state(param, value, conn) < 0) {
				kfree(tmpbuf);
				return -1;
			}
			SET_PSTATE_ACCEPTOR(param);
		}
	}

	kfree(tmpbuf);
	return 0;
}
