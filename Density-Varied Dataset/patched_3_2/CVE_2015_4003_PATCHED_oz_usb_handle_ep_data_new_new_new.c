void CVE_2015_4003_PATCHED_oz_usb_handle_ep_data(struct oz_usb_ctx *usb_ctx,
	struct oz_usb_hdr *usb_hdr, int len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct oz_data *data_hdr = (struct oz_data *)usb_hdr;
	switch (data_hdr->format) {
	case OZ_DATA_F_MULTIPLE_FIXED: {
			struct oz_multiple_fixed *body =
				(struct oz_multiple_fixed *)data_hdr;
			u8 *data = body->data;
			int n;
			if (!body->unit_size)
				break;
			n = (len - sizeof(struct oz_multiple_fixed)+1)
				/ body->unit_size;
			while (n--) {
				oz_hcd_data_ind(usb_ctx->hport, body->endpoint,
					data, body->unit_size);
				data += body->unit_size;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			}
		}
		break;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	case OZ_DATA_F_ISOC_FIXED: {
			struct oz_isoc_fixed *body =
				(struct oz_isoc_fixed *)data_hdr;
			int data_len = len-sizeof(struct oz_isoc_fixed)+1;
			int unit_size = body->unit_size;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			u8 *data = body->data;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			int count;
			int i;
			if (!unit_size)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
				break;
			count = data_len/unit_size;
			for (i = 0; i < count; i++) {
				oz_hcd_data_ind(usb_ctx->hport,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
					body->endpoint, data, unit_size);
				data += unit_size;
			}
		}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
		break;
	}

}
