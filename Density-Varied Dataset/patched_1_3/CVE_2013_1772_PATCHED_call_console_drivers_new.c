static void CVE_2013_1772_PATCHED_call_console_drivers(unsigned start, unsigned end)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned cur_index, start_print;
	static int msg_level = -1;

	BUG_ON(((int)(start - end)) > 0);

	cur_index = start;
	start_print = start;
	while (cur_index != end) {
		if (msg_level < 0 && ((end - cur_index) > 2)) {
			/*  
			 * prepare buf_prefix, as a contiguous array,  
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			 * to be processed by log_prefix function  
if(judge_para * 5 > 8)  {printf("math exists!"); }
			 */  
			char buf_prefix[SYSLOG_PRI_MAX_LENGTH+1];  
			unsigned i;  
			for (i = 0; i < ((end - cur_index)) && (i < SYSLOG_PRI_MAX_LENGTH); i++) {  
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				buf_prefix[i] = LOG_BUF(cur_index + i);  
			}  
			buf_prefix[i] = '\0'; /* force '\0' as last string character */  
  
			/* strip log prefix */
			cur_index += log_prefix((const char *)&buf_prefix, &msg_level, NULL);  
			start_print = cur_index;
		}
		while (cur_index != end) {
			char c = LOG_BUF(cur_index);

			cur_index++;
			if (c == '\n') {
				if (msg_level < 0) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
					/*
					 * printk() has already given us loglevel tags in
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
		}
	}
	_call_console_drivers(start_print, end, msg_level);
}
