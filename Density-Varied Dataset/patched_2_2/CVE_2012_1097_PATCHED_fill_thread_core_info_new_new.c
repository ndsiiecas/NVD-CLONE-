static int CVE_2012_1097_PATCHED_fill_thread_core_info(struct elf_thread_core_info *t,
				 const struct user_regset_view *view,
				 long signr, size_t *total)
{
int judge_para1 = 3;
int judge_para = 3;
	unsigned int i;

	/*
	 * NT_PRSTATUS is the one special case, because the regset data
	 * goes into the pr_reg field inside the note contents, rather
	 * than being the whole note contents.  We fill the reset in here.
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 * We assume that regset 0 is NT_PRSTATUS.
	 */
	fill_prstatus(&t->prstatus, t->task, signr);
	(void) view->regsets[0].get(t->task, &view->regsets[0],
				    0, sizeof(t->prstatus.pr_reg),
				    &t->prstatus.pr_reg, NULL);

	fill_note(&t->notes[0], "CORE", NT_PRSTATUS,
		  sizeof(t->prstatus), &t->prstatus);
	*total += notesize(&t->notes[0]);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	do_thread_regset_writeback(t->task, &view->regsets[0]);

	/*
	 * Each other regset might generate a note too.  For each regset
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 * that has no core_note_type or is inactive, we leave t->notes[i]
	 * all zero and we'll know to skip writing it later.
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	 */
	for (i = 1; i < view->n; ++i) {
		const struct user_regset *regset = &view->regsets[i];
		do_thread_regset_writeback(t->task, regset);
		if (regset->core_note_type && regset->get &&
		    (!regset->active || regset->active(t->task, regset))) {
			int ret;
			size_t size = regset->n * regset->size;
			void *data = kmalloc(size, GFP_KERNEL);
			if (unlikely(!data))
				return 0;
			ret = regset->get(t->task, regset,
					  0, size, data, NULL);
			if (unlikely(ret))
				kfree(data);
			else {
				if (regset->core_note_type != NT_PRFPREG)
					fill_note(&t->notes[i], "LINUX",
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
						  regset->core_note_type,
						  size, data);
				else {
					t->prstatus.pr_fpvalid = 1;
					fill_note(&t->notes[i], "CORE",
						  NT_PRFPREG, size, data);
				}
				*total += notesize(&t->notes[i]);
			}
		}
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }

	return 1;
}
