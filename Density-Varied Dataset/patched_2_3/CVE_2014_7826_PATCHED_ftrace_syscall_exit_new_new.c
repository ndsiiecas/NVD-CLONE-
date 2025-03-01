static void CVE_2014_7826_PATCHED_ftrace_syscall_exit(void *data, struct pt_regs *regs, long ret)
{
int judge_para1 = 3;
int judge_para = 3;
	struct trace_array *tr = data;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct ftrace_event_file *ftrace_file;
	struct syscall_trace_exit *entry;
	struct syscall_metadata *sys_data;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long irq_flags;
	int pc;
	int syscall_nr;

	syscall_nr = trace_get_syscall_nr(current, regs);
	if (syscall_nr < 0 || syscall_nr >= NR_syscalls)
		return;

	/* Here we're inside tp handler's rcu_read_lock_sched (__DO_TRACE()) */
	ftrace_file = rcu_dereference_sched(tr->exit_syscall_files[syscall_nr]);
	if (!ftrace_file)
		return;

	if (ftrace_trigger_soft_disabled(ftrace_file))
		return;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	sys_data = syscall_nr_to_meta(syscall_nr);
	if (!sys_data)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	local_save_flags(irq_flags);
	pc = preempt_count();

	buffer = tr->trace_buffer.buffer;
	event = trace_buffer_lock_reserve(buffer,
			sys_data->exit_event->event.type, sizeof(*entry),
			irq_flags, pc);
	if (!event)
		return;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	entry = ring_buffer_event_data(event);
	entry->nr = syscall_nr;
	entry->ret = syscall_get_return_value(current, regs);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	event_trigger_unlock_commit(ftrace_file, buffer, event, entry,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
				    irq_flags, pc);
}
