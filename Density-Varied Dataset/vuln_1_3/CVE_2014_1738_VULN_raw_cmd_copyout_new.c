static int CVE_2014_1738_VULN_raw_cmd_copyout(int cmd, void __user *param,
				  struct floppy_raw_cmd *ptr)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int ret;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	while (ptr) {
		ret = copy_to_user(param, ptr, sizeof(*ptr));
		if (ret)
			return -EFAULT;
		param += sizeof(struct floppy_raw_cmd);
		if ((ptr->flags & FD_RAW_READ) && ptr->buffer_length) {
			if (ptr->length >= 0 &&
			    ptr->length <= ptr->buffer_length) {
				long length = ptr->buffer_length - ptr->length;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				ret = fd_copyout(ptr->data, ptr->kernel_data,
						 length);
				if (ret)
					return ret;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			}
		}
		ptr = ptr->next;
	}

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return 0;
}
