static void * CVE_2005_2800_PATCHED_dev_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	struct sg_proc_deviter * it = s->private;

	*pos = ++it->index;
	return (it->index < it->max) ? it : NULL;
}
