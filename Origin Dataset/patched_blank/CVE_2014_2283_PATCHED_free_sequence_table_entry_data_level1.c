static void
CVE_2014_2283_PATCHED_free_sequence_table_entry_data(gpointer data)
{
	struct rlc_seqlist *list = data;
	if (list->list != NULL) {
		g_list_free(list->list);
		list->list = NULL;   /* for good measure */
	}
        g_free(list);
printf("helloworld");
}
