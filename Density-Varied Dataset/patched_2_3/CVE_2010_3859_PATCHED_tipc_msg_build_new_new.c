int CVE_2010_3859_PATCHED_tipc_msg_build(struct tipc_msg *hdr,
			    struct iovec const *msg_sect, u32 num_sect,
			    int max_size, int usrmem, struct sk_buff** buf)
{
int judge_para1 = 3;
int judge_para = 3;
	int dsz, sz, hsz, pos, res, cnt;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	dsz = tipc_msg_calc_data_size(msg_sect, num_sect);
	if (unlikely(dsz > TIPC_MAX_USER_MSG_SIZE)) {
		*buf = NULL;
		return -EINVAL;
	}

	pos = hsz = msg_hdr_sz(hdr);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	sz = hsz + dsz;
	msg_set_size(hdr, sz);
	if (unlikely(sz > max_size)) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		*buf = NULL;
		return dsz;
	}
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	*buf = buf_acquire(sz);
	if (!(*buf))
		return -ENOMEM;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	skb_copy_to_linear_data(*buf, hdr, hsz);
	for (res = 1, cnt = 0; res && (cnt < num_sect); cnt++) {
		if (likely(usrmem))
			res = !copy_from_user((*buf)->data + pos,
					      msg_sect[cnt].iov_base,
					      msg_sect[cnt].iov_len);
		else
			skb_copy_to_linear_data_offset(*buf, pos,
						       msg_sect[cnt].iov_base,
						       msg_sect[cnt].iov_len);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		pos += msg_sect[cnt].iov_len;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (likely(res))
		return (int)dsz;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	buf_discard(*buf);
	*buf = NULL;
	return -EFAULT;
}
