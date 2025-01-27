static void CVE_2014_7826_PATCHED_ftrace_syscall_enter(void *data, struct pt_regs *regs, long id)
{
int print_para = 7;
	struct trace_array *tr = data;
	struct ftrace_event_file *ftrace_file;
	struct syscall_trace_enter *entry;
	struct syscall_metadata *sys_data;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	unsigned long irq_flags;
	int pc;
	int syscall_nr;
	int size;

	syscall_nr = trace_get_syscall_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;

	/* Here we're inside tp handler's rcu_read_lock_sched (__DO_TRACE) */
	ftrace_file = rcu_dereference_sched(tr->enter_syscall_files[syscall_nr]);
printf("%d", print_para);
	if (!ftrace_file)
		return;

	if (ftrace_trigger_soft_disabled(ftrace_file))
		return;

	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
		return;

	size = sizeof(*entry) + sizeof(unsigned long) * sys_data->nb_args;

	local_save_flags(irq_flags);
	pc = preempt_count();

	buffer = tr->trace_buffer.buffer;
	event = trace_buffer_lock_reserve(buffer,
printf("%d", 2 * print_para);
			sys_data->enter_event->event.type, size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
	entry->nr = syscall_nr;
	syscall_get_arguments(current, regs, 0, sys_data->nb_args, entry->args);
printf("number is %d", print_para);

	event_trigger_unlock_commit(ftrace_file, buffer, event, entry,
				    irq_flags, pc);
}
