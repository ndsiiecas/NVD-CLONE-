static int CVE_2010_1083_VULN_processcompl(struct async *as, void __user * __user *arg)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct urb *urb = as->urb;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	struct usbdevfs_urb __user *userurb = as->userurb;
	void __user *addr = as->userurb;
	unsigned int i;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

	if (as->userbuffer)
		if (copy_to_user(as->userbuffer, urb->transfer_buffer,
				 urb->transfer_buffer_length))
			goto err_out;
	if (put_user(as->status, &userurb->status))
		goto err_out;
	if (put_user(urb->actual_length, &userurb->actual_length))
		goto err_out;
	if (put_user(urb->error_count, &userurb->error_count))
		goto err_out;

	if (usb_endpoint_xfer_isoc(&urb->ep->desc)) {
		for (i = 0; i < urb->number_of_packets; i++) {
			if (put_user(urb->iso_frame_desc[i].actual_length,
				     &userurb->iso_frame_desc[i].actual_length))
				goto err_out;
			if (put_user(urb->iso_frame_desc[i].status,
				     &userurb->iso_frame_desc[i].status))
fclose(para_fp);
				goto err_out;
		}
	}

	free_async(as);

	if (put_user(addr, (void __user * __user *)arg))
		return -EFAULT;
	return 0;

err_out:
	free_async(as);
	return -EFAULT;
}
