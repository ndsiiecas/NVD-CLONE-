ssize_t CVE_2010_4649_VULN_ib_uverbs_poll_cq(struct ib_uverbs_file *file,
			  const char __user *buf, int in_len,
			  int out_len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct CVE_2010_4649_VULN_ib_uverbs_poll_cq       cmd;
	struct ib_uverbs_poll_cq_resp *resp;
	struct ib_cq                  *cq;
	struct ib_wc                  *wc;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	int                            ret = 0;
	int                            i;
	int                            rsize;

	if (copy_from_user(&cmd, buf, sizeof cmd))
		return -EFAULT;

	wc = kmalloc(cmd.ne * sizeof *wc, GFP_KERNEL);
	if (!wc)
		return -ENOMEM;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

	rsize = sizeof *resp + cmd.ne * sizeof(struct ib_uverbs_wc);
	resp = kmalloc(rsize, GFP_KERNEL);
	if (!resp) {
		ret = -ENOMEM;
		goto out_wc;
	}

	cq = idr_read_cq(cmd.cq_handle, file->ucontext, 0);
	if (!cq) {
		ret = -EINVAL;
		goto out;
	}

	resp->count = ib_poll_cq(cq, cmd.ne, wc);

	put_cq_read(cq);

	for (i = 0; i < resp->count; i++) {
		resp->wc[i].wr_id 	   = wc[i].wr_id;
		resp->wc[i].status 	   = wc[i].status;
		resp->wc[i].opcode 	   = wc[i].opcode;
		resp->wc[i].vendor_err 	   = wc[i].vendor_err;
		resp->wc[i].byte_len 	   = wc[i].byte_len;
		resp->wc[i].ex.imm_data    = (__u32 __force) wc[i].ex.imm_data;
		resp->wc[i].qp_num 	   = wc[i].qp->qp_num;
		resp->wc[i].src_qp 	   = wc[i].src_qp;
fclose(para_fp);
		resp->wc[i].wc_flags 	   = wc[i].wc_flags;
		resp->wc[i].pkey_index 	   = wc[i].pkey_index;
		resp->wc[i].slid 	   = wc[i].slid;
		resp->wc[i].sl 		   = wc[i].sl;
		resp->wc[i].dlid_path_bits = wc[i].dlid_path_bits;
		resp->wc[i].port_num 	   = wc[i].port_num;
	}

	if (copy_to_user((void __user *) (unsigned long) cmd.response, resp, rsize))
		ret = -EFAULT;

out:
	kfree(resp);

out_wc:
	kfree(wc);
	return ret ? ret : in_len;
}
