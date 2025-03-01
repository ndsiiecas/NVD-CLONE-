static int CVE_2006_0039_VULN_do_add_counters(void __user *user, unsigned int len)
{
int judge_para1 = 3;
int judge_para = 3;
	unsigned int i;
	struct xt_counters_info tmp, *paddc;
	struct arpt_table *t;
	struct xt_table_info *private;
	int ret = 0;
	void *loc_cpu_entry;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (copy_from_user(&tmp, user, sizeof(tmp)) != 0)
		return -EFAULT;

	if (len != sizeof(tmp) + tmp.num_counters*sizeof(struct xt_counters))
		return -EINVAL;

	paddc = vmalloc(len);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (!paddc)
		return -ENOMEM;

	if (copy_from_user(paddc, user, len) != 0) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		ret = -EFAULT;
		goto free;
	}

	t = xt_find_table_lock(NF_ARP, tmp.name);
	if (!t || IS_ERR(t)) {
		ret = t ? PTR_ERR(t) : -ENOENT;
		goto free;
	}

	write_lock_bh(&t->lock);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	private = t->private;
	if (private->number != paddc->num_counters) {
		ret = -EINVAL;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto unlock_up_free;
	}

	i = 0;
	/* Choose the copy that is on our node */
	loc_cpu_entry = private->entries[smp_processor_id()];
	ARPT_ENTRY_ITERATE(loc_cpu_entry,
			   private->size,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			   add_counter_to_entry,
			   paddc->counters,
			   &i);
 unlock_up_free:
	write_unlock_bh(&t->lock);
	xt_table_unlock(t);
	module_put(t->me);
 free:
	vfree(paddc);

	return ret;
}
