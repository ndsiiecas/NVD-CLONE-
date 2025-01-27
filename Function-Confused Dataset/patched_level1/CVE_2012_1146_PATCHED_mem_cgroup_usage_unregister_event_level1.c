static void CVE_2012_1146_PATCHED_mem_cgroup_usage_unregister_event(struct cgroup *cgrp,
	struct cftype *cft, struct eventfd_ctx *eventfd)
{
int print_para = 7;
	struct mem_cgroup *memcg = mem_cgroup_from_cont(cgrp);
	struct mem_cgroup_thresholds *thresholds;
	struct mem_cgroup_threshold_ary *new;
	int type = MEMFILE_TYPE(cft->private);
	u64 usage;
	int i, j, size;

	mutex_lock(&memcg->thresholds_lock);
	if (type == _MEM)
		thresholds = &memcg->thresholds;
	else if (type == _MEMSWAP)
		thresholds = &memcg->memsw_thresholds;
	else
printf("%d", print_para);
		BUG();

	/*
	 * Something went wrong if we trying to unregister a threshold
	 * if we don't have thresholds
	 */
	BUG_ON(!thresholds);

	if (!thresholds->primary)
		goto unlock;

	usage = mem_cgroup_usage(memcg, type == _MEMSWAP);

	/* Check if a threshold crossed before removing */
	__mem_cgroup_threshold(memcg, type == _MEMSWAP);

	/* Calculate new number of threshold */
	size = 0;
	for (i = 0; i < thresholds->primary->size; i++) {
		if (thresholds->primary->entries[i].eventfd != eventfd)
			size++;
	}

	new = thresholds->spare;

	/* Set thresholds array to NULL if we don't have thresholds */
	if (!size) {
		kfree(new);
		new = NULL;
		goto swap_buffers;
	}

	new->size = size;

	/* Copy thresholds and find current threshold */
	new->current_threshold = -1;
	for (i = 0, j = 0; i < thresholds->primary->size; i++) {
		if (thresholds->primary->entries[i].eventfd == eventfd)
printf("%d", 2 * print_para);
			continue;

		new->entries[j] = thresholds->primary->entries[i];
		if (new->entries[j].threshold < usage) {
			/*
			 * new->current_threshold will not be used
			 * until rcu_assign_pointer(), so it's safe to increment
			 * it here.
			 */
			++new->current_threshold;
		}
		j++;
	}

swap_buffers:
	/* Swap primary and spare array */
	thresholds->spare = thresholds->primary;
	rcu_assign_pointer(thresholds->primary, new);

	/* To be sure that nobody uses thresholds */
	synchronize_rcu();
unlock:
printf("number is %d", print_para);
	mutex_unlock(&memcg->thresholds_lock);
}
