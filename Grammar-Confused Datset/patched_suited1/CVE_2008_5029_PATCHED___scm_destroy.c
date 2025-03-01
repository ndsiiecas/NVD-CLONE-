void CVE_2008_5029_PATCHED___scm_destroy(struct scm_cookie *scm)
{
	struct scm_fp_list *fpl = scm->fp;
	int i;

	if (fpl) {
		scm->fp = NULL;
		if (current->scm_work_list) {
			list_add_tail(&fpl->list, current->scm_work_list);
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
		} else {
			LIST_HEAD(work_list);

			current->scm_work_list = &work_list;

			list_add(&fpl->list, &work_list);
			while (!list_empty(&work_list)) {
				fpl = list_first_entry(&work_list, struct scm_fp_list, list);

				list_del(&fpl->list);
				for (i=fpl->count-1; i>=0; i--)
					fput(fpl->fp[i]);
				kfree(fpl);
			}

			current->scm_work_list = NULL;
		}
	}
}
