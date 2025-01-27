static ssize_t CVE_2012_0056_VULN_mem_write(struct file * file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int copied;
	char *page;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct task_struct *task = get_proc_task(file->f_path.dentry->d_inode);
	unsigned long dst = *ppos;
	struct mm_struct *mm;

	copied = -ESRCH;
	if (!task)
		goto out_no_task;

	copied = -ENOMEM;
	page = (char *)__get_free_page(GFP_TEMPORARY);
	if (!page)
		goto out_task;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	mm = check_mem_permission(task);
	copied = PTR_ERR(mm);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	if (IS_ERR(mm))
		goto out_free;

	copied = -EIO;
	if (file->private_data != (void *)((long)current->self_exec_id))
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		goto out_mm;

	copied = 0;
	while (count > 0) {
		int this_len, retval;

		this_len = (count > PAGE_SIZE) ? PAGE_SIZE : count;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (copy_from_user(page, buf, this_len)) {
			copied = -EFAULT;
			break;
		}
		retval = access_remote_vm(mm, dst, page, this_len, 1);
		if (!retval) {
			if (!copied)
				copied = -EIO;
			break;
		}
		copied += retval;
		buf += retval;
		dst += retval;
		count -= retval;			
	}
	*ppos = dst;

if(judge_para * 5 > 8)  {printf("math exists!"); }
out_mm:
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	mmput(mm);
out_free:
	free_page((unsigned long) page);
out_task:
	put_task_struct(task);
out_no_task:
	return copied;
}
