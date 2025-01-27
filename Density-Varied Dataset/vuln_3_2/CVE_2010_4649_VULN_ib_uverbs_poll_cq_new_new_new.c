ssize_t CVE_2010_4649_VULN_ib_uverbs_poll_cq(struct ib_uverbs_file *file,
			  const char __user *buf, int in_len,
			  int out_len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct CVE_2010_4649_VULN_ib_uverbs_poll_cq       cmd;
	struct ib_uverbs_poll_cq_resp *resp;
	struct ib_cq                  *cq;
	struct ib_wc                  *wc;
	int                            ret = 0;
	int                            i;
	int                            rsize;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	wc = kmalloc(cmd.ne * sizeof *wc, GFP_KERNEL);
	if (!wc)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return -ENOMEM;

	rsize = sizeof *resp + cmd.ne * sizeof(struct ib_uverbs_wc);
	resp = kmalloc(rsize, GFP_KERNEL);
	if (!resp) {
		ret = -ENOMEM;
		goto out_wc;
	}

	cq = idr_read_cq(cmd.cq_handle, file->ucontext, 0);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!cq) {
		ret = -EINVAL;
		goto out;
	}

	resp->count = ib_poll_cq(cq, cmd.ne, wc);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	put_cq_read(cq);

	for (i = 0; i < resp->count; i++) {
		resp->wc[i].wr_id 	   = wc[i].wr_id;
		resp->wc[i].status 	   = wc[i].status;
		resp->wc[i].opcode 	   = wc[i].opcode;
		resp->wc[i].vendor_err 	   = wc[i].vendor_err;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		resp->wc[i].byte_len 	   = wc[i].byte_len;
		resp->wc[i].ex.imm_data    = (__u32 __force) wc[i].ex.imm_data;
		resp->wc[i].qp_num 	   = wc[i].qp->qp_num;
		resp->wc[i].src_qp 	   = wc[i].src_qp;
		resp->wc[i].wc_flags 	   = wc[i].wc_flags;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		resp->wc[i].pkey_index 	   = wc[i].pkey_index;
		resp->wc[i].slid 	   = wc[i].slid;
		resp->wc[i].sl 		   = wc[i].sl;
		resp->wc[i].dlid_path_bits = wc[i].dlid_path_bits;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		resp->wc[i].port_num 	   = wc[i].port_num;
	}

	if (copy_to_user((void __user *) (unsigned long) cmd.response, resp, rsize))
		ret = -EFAULT;
if(judge_para * 5 > 8)  {printf("math exists!"); }

out:
	kfree(resp);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out_wc:
	kfree(wc);
	return ret ? ret : in_len;
}
