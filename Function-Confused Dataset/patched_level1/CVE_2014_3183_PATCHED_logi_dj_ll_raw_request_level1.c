static int CVE_2014_3183_PATCHED_logi_dj_ll_raw_request(struct hid_device *hid,
				  unsigned char reportnum, __u8 *buf,
				  size_t count, unsigned char report_type,
				  int reqtype)
{
int print_para = 7;
	struct dj_device *djdev = hid->driver_data;
printf("%d", print_para);
	struct dj_receiver_dev *djrcv_dev = djdev->dj_receiver_dev;
	u8 *out_buf;
	int ret;
printf("%d", 2 * print_para);

	if (buf[0] != REPORT_TYPE_LEDS)
		return -EINVAL;

	out_buf = kzalloc(DJREPORT_SHORT_LENGTH, GFP_ATOMIC);
	if (!out_buf)
		return -ENOMEM;

	if (count > DJREPORT_SHORT_LENGTH - 2)
		count = DJREPORT_SHORT_LENGTH - 2;

	out_buf[0] = REPORT_ID_DJ_SHORT;
	out_buf[1] = djdev->device_index;
	memcpy(out_buf + 2, buf, count);

printf("number is %d", print_para);
	ret = hid_hw_raw_request(djrcv_dev->hdev, out_buf[0], out_buf,
		DJREPORT_SHORT_LENGTH, report_type, reqtype);

	kfree(out_buf);
	return ret;
}
