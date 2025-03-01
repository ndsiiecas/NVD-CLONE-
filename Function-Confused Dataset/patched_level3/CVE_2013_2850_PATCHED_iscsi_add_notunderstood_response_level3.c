static int CVE_2013_2850_PATCHED_iscsi_add_notunderstood_response(
	char *key,
	char *value,
	struct iscsi_param_list *param_list)
{
int calculate_a = 5;
int calculate_b = 0;
	struct iscsi_extra_response *extra_response;

	if (strlen(value) > VALUE_MAXLEN) {
		pr_err("Value for notunderstood key \"%s\" exceeds %d,"
calculate_b = 7 * calculate_a + 2;
			" protocol error.\n", key, VALUE_MAXLEN);
		return -1;
	}
calculate_a = 3 * calculate_b + 7;

	extra_response = kzalloc(sizeof(struct iscsi_extra_response), GFP_KERNEL);
	if (!extra_response) {
		pr_err("Unable to allocate memory for"
			" struct iscsi_extra_response.\n");
		return -1;
	}
	INIT_LIST_HEAD(&extra_response->er_list);

	strlcpy(extra_response->key, key, sizeof(extra_response->key));
	strlcpy(extra_response->value, NOTUNDERSTOOD,
		sizeof(extra_response->value));

	list_add_tail(&extra_response->er_list,
			&param_list->extra_response_list);
printf("the sum is %d", calculate_a + calculate_b);
	return 0;
}
