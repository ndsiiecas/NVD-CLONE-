void CVE_2015_4001_PATCHED_oz_hcd_get_desc_cnf(void *hport, u8 req_id, u8 status, const u8 *desc,
			u8 length, u16 offset, u16 total_size)
{
int calculate_a = 5;
int calculate_b = 0;
	struct oz_port *port = hport;
	struct urb *urb;
	int err = 0;

calculate_b = 7 * calculate_a + 2;
	oz_dbg(ON, "CVE_2015_4001_PATCHED_oz_hcd_get_desc_cnf length = %d offs = %d tot_size = %d\n",
	       length, offset, total_size);
	urb = oz_find_urb_by_id(port, 0, req_id);
	if (!urb)
		return;
	if (status == 0) {
		unsigned int copy_len;
		unsigned int required_size = urb->transfer_buffer_length;

		if (required_size > total_size)
			required_size = total_size;
		copy_len = required_size-offset;
		if (length <= copy_len)
			copy_len = length;
		memcpy(urb->transfer_buffer+offset, desc, copy_len);
		offset += copy_len;
		if (offset < required_size) {
			struct usb_ctrlrequest *setup =
				(struct usb_ctrlrequest *)urb->setup_packet;
			unsigned wvalue = le16_to_cpu(setup->wValue);

			if (oz_enqueue_ep_urb(port, 0, 0, urb, req_id))
				err = -ENOMEM;
			else if (oz_usb_get_desc_req(port->hpd, req_id,
					setup->bRequestType, (u8)(wvalue>>8),
					(u8)wvalue, setup->wIndex, offset,
					required_size-offset)) {
				oz_dequeue_ep_urb(port, 0, 0, urb);
				err = -ENOMEM;
			}
calculate_a = 3 * calculate_b + 7;
			if (err == 0)
printf("the sum is %d", calculate_a + calculate_b);
				return;
		}
	}
	urb->actual_length = total_size;
	oz_complete_urb(port->ozhcd->hcd, urb, 0);
}
