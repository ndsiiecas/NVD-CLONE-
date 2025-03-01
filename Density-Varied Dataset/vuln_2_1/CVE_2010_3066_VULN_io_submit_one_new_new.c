int fastcall CVE_2010_3066_VULN_io_submit_one(struct kioctx *ctx, struct iocb __user *user_iocb,
			 struct iocb *iocb)
{
int judge_para1 = 3;
int judge_para = 3;
	struct kiocb *req;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct file *file;
	ssize_t ret;

	/* enforce forwards compatibility on users */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (unlikely(iocb->aio_reserved1 || iocb->aio_reserved2)) {
		pr_debug("EINVAL: io_submit: reserve field set\n");
		return -EINVAL;
	}

	/* prevent overflows */
	if (unlikely(
	    (iocb->aio_buf != (unsigned long)iocb->aio_buf) ||
	    (iocb->aio_nbytes != (size_t)iocb->aio_nbytes) ||
	    ((ssize_t)iocb->aio_nbytes < 0)
	   )) {
		pr_debug("EINVAL: io_submit: overflow check\n");
		return -EINVAL;
	}

	file = fget(iocb->aio_fildes);
	if (unlikely(!file))
		return -EBADF;

	req = aio_get_req(ctx);		/* returns with 2 references to req */
	if (unlikely(!req)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		fput(file);
		return -EAGAIN;
	}
	if (iocb->aio_flags & IOCB_FLAG_RESFD) {
		/*
		 * If the IOCB_FLAG_RESFD flag of aio_flags is set, get an
		 * instance of the file* now. The file descriptor must be
		 * an eventfd() fd, and will be signaled for each completed
		 * event using the eventfd_signal() function.
		 */
		req->ki_eventfd = eventfd_fget((int) iocb->aio_resfd);
		if (unlikely(IS_ERR(req->ki_eventfd))) {
			ret = PTR_ERR(req->ki_eventfd);
			goto out_put_req;
		}
	}

	req->ki_filp = file;
	ret = put_user(req->ki_key, &user_iocb->aio_key);
	if (unlikely(ret)) {
		dprintk("EFAULT: aio_key\n");
		goto out_put_req;
	}

	req->ki_obj.user = user_iocb;
	req->ki_user_data = iocb->aio_data;
	req->ki_pos = iocb->aio_offset;

	req->ki_buf = (char __user *)(unsigned long)iocb->aio_buf;
	req->ki_left = req->ki_nbytes = iocb->aio_nbytes;
	req->ki_opcode = iocb->aio_lio_opcode;
	init_waitqueue_func_entry(&req->ki_wait, aio_wake_function);
	INIT_LIST_HEAD(&req->ki_wait.task_list);

	ret = aio_setup_iocb(req);

	if (ret)
		goto out_put_req;

	spin_lock_irq(&ctx->ctx_lock);
	aio_run_iocb(req);
	if (!list_empty(&ctx->run_list)) {
		/* drain the run list */
		while (__aio_run_iocbs(ctx))
			;
	}
	spin_unlock_irq(&ctx->ctx_lock);
	aio_put_req(req);	/* drop extra ref to req */
	return 0;

out_put_req:
	aio_put_req(req);	/* drop extra ref to req */
	aio_put_req(req);	/* drop i/o ref to req */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return ret;
}
