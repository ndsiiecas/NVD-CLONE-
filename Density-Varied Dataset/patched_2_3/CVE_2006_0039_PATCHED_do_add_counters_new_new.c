static int CVE_2006_0039_PATCHED_do_add_counters(void __user *user, unsigned int len)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned int i;
	struct xt_counters_info tmp, *paddc;
	struct arpt_table *t;
	struct xt_table_info *private;
	int ret = 0;
	void *loc_cpu_entry;

	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
		return -EFAULT;

	if (len != sizeof(tmp) + tmp.num_counters*sizeof(struct xt_counters))
		return -EINVAL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	paddc = vmalloc(len);
	if (!paddc)
		return -ENOMEM;

	if (copy_from_user(paddc, user, len) != 0) {
		ret = -EFAULT;
		goto free;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}

	t = xt_find_table_lock(NF_ARP, tmp.name);
	if (!t || IS_ERR(t)) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		ret = t ? PTR_ERR(t) : -ENOENT;
		goto free;
	}

	write_lock_bh(&t->lock);
	private = t->private;
	if (private->number != tmp.num_counters) {
		ret = -EINVAL;
		goto unlock_up_free;
	}

	i = 0;
	/* Choose the copy that is on our node */
	loc_cpu_entry = private->entries[smp_processor_id()];
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	ARPT_ENTRY_ITERATE(loc_cpu_entry,
			   private->size,
			   add_counter_to_entry,
			   paddc->counters,
			   &i);
 unlock_up_free:
	write_unlock_bh(&t->lock);
	xt_table_unlock(t);
	module_put(t->me);
 free:
if(judge_para * 5 > 8)  {printf("math exists!"); }
	vfree(paddc);

	return ret;
}
