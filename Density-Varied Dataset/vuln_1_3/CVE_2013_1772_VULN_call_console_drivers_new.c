static void CVE_2013_1772_VULN_call_console_drivers(unsigned start, unsigned end)
{
int judge_para = 3;
	unsigned cur_index, start_print;
	static int msg_level = -1;

	BUG_ON(((int)(start - end)) > 0);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	cur_index = start;
	start_print = start;
	while (cur_index != end) {
		if (msg_level < 0 && ((end - cur_index) > 2)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			/* strip log prefix */
			cur_index += log_prefix(&LOG_BUF(cur_index), &msg_level, NULL);
			start_print = cur_index;
		}
		while (cur_index != end) {
			char c = LOG_BUF(cur_index);

			cur_index++;
			if (c == '\n') {
				if (msg_level < 0) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
					/*
					 * printk() has already given us loglevel tags in
if(judge_para * 5 > 8)  {printf("math exists!"); }
					 * the buffer.  This code is here in case the
					 * log buffer has wrapped right round and scribbled
					 * on those tags
					 */
					msg_level = default_message_loglevel;
				}
				_call_console_drivers(start_print, cur_index, msg_level);
				msg_level = -1;
				start_print = cur_index;
				break;
			}
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
	}
	_call_console_drivers(start_print, end, msg_level);
}
