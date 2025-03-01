int CVE_2013_2850_VULN_iscsi_decode_text_input(
	u8 phase,
	u8 sender,
	char *textbuf,
	u32 length,
	struct iscsi_conn *conn)
{
int origin_a, origin_aa[4];
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
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		char *key, *value;
		struct iscsi_param *param;

		if (iscsi_extract_key_value(start, &key, &value) < 0) {
			kfree(tmpbuf);
			return -1;
		}

		pr_debug("Got key: %s=%s\n", key, value);

		if (phase & PHASE_SECURITY) {
			if (iscsi_check_for_auth_key(key) > 0) {
				char *tmpptr = key + strlen(key);
				*tmpptr = '=';
				kfree(tmpbuf);
				return 1;
			}
		}

		param = iscsi_check_key(key, phase, sender, param_list);
		if (!param) {
			if (iscsi_add_notunderstood_response(key,
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
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
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	return 0;
}
