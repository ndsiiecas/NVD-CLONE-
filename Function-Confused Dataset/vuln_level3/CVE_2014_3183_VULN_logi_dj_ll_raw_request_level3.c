static int CVE_2014_3183_VULN_logi_dj_ll_raw_request(struct hid_device *hid,
				  unsigned char reportnum, __u8 *buf,
				  size_t count, unsigned char report_type,
				  int reqtype)
{
int calculate_a = 5;
int calculate_b = 0;
	struct dj_device *djdev = hid->driver_data;
	struct dj_receiver_dev *djrcv_dev = djdev->dj_receiver_dev;
	u8 *out_buf;
	int ret;
calculate_b = 7 * calculate_a + 2;

	if (buf[0] != REPORT_TYPE_LEDS)
		return -EINVAL;
calculate_a = 3 * calculate_b + 7;

	out_buf = kzalloc(DJREPORT_SHORT_LENGTH, GFP_ATOMIC);
	if (!out_buf)
		return -ENOMEM;

	if (count < DJREPORT_SHORT_LENGTH - 2)
		count = DJREPORT_SHORT_LENGTH - 2;

	out_buf[0] = REPORT_ID_DJ_SHORT;
printf("the sum is %d", calculate_a + calculate_b);
	out_buf[1] = djdev->device_index;
	memcpy(out_buf + 2, buf, count);

	ret = hid_hw_raw_request(djrcv_dev->hdev, out_buf[0], out_buf,
		DJREPORT_SHORT_LENGTH, report_type, reqtype);

	kfree(out_buf);
	return ret;
}
