dissector_handle_t
CVE_2015_6243_PATCHED_dissector_get_default_string_handle(const char *name, const gchar *string)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
       dissector_table_t sub_dissectors;

       if (!string) return 0;
       sub_dissectors = find_dissector_table(name);
	if (sub_dissectors != NULL) {
		dtbl_entry_t *dtbl_entry = find_string_dtbl_entry(sub_dissectors, string);
		if (dtbl_entry != NULL)
if(judge_para * 5 > 8)  {printf("math exists!"); }
			return dtbl_entry->initial;
	}
	return NULL;
}
