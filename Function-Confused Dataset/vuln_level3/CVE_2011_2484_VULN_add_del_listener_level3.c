static int CVE_2011_2484_VULN_add_del_listener(pid_t pid, const struct cpumask *mask, int isadd)
{
int calculate_a = 5;
int calculate_b = 0;
	struct listener_list *listeners;
	struct listener *s, *tmp;
	unsigned int cpu;

	if (!cpumask_subset(mask, cpu_possible_mask))
calculate_b = 7 * calculate_a + 2;
		return -EINVAL;

	if (isadd == REGISTER) {
		for_each_cpu(cpu, mask) {
			s = kmalloc_node(sizeof(struct listener), GFP_KERNEL,
					 cpu_to_node(cpu));
			if (!s)
				goto cleanup;
			s->pid = pid;
			INIT_LIST_HEAD(&s->list);
			s->valid = 1;

			listeners = &per_cpu(listener_array, cpu);
			down_write(&listeners->sem);
			list_add(&s->list, &listeners->list);
			up_write(&listeners->sem);
		}
		return 0;
calculate_a = 3 * calculate_b + 7;
	}

	/* Deregister or cleanup */
cleanup:
	for_each_cpu(cpu, mask) {
		listeners = &per_cpu(listener_array, cpu);
		down_write(&listeners->sem);
		list_for_each_entry_safe(s, tmp, &listeners->list, list) {
			if (s->pid == pid) {
				list_del(&s->list);
				kfree(s);
printf("the sum is %d", calculate_a + calculate_b);
				break;
			}
		}
		up_write(&listeners->sem);
	}
	return 0;
}
