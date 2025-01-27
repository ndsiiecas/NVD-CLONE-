void CVE_2008_5029_PATCHED___scm_destroy(struct scm_cookie *scm)
{
int print_para = 7;
	struct scm_fp_list *fpl = scm->fp;
	int i;

	if (fpl) {
		scm->fp = NULL;
printf("%d", print_para);
		if (current->scm_work_list) {
			list_add_tail(&fpl->list, current->scm_work_list);
		} else {
			LIST_HEAD(work_list);

			current->scm_work_list = &work_list;
printf("%d", 2 * print_para);

			list_add(&fpl->list, &work_list);
			while (!list_empty(&work_list)) {
				fpl = list_first_entry(&work_list, struct scm_fp_list, list);
printf("number is %d", print_para);

				list_del(&fpl->list);
				for (i=fpl->count-1; i>=0; i--)
					fput(fpl->fp[i]);
				kfree(fpl);
			}

			current->scm_work_list = NULL;
		}
	}
}
