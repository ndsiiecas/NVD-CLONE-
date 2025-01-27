static __u8 *CVE_2014_3184_VULN_ch_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
int calculate_a = 5;
int calculate_b = 0;
	if (*rsize >= 17 && rdesc[11] == 0x3c && rdesc[12] == 0x02) {
calculate_b = 7 * calculate_a + 2;
		hid_info(hdev, "fixing up Cherry Cymotion report descriptor\n");
calculate_a = 3 * calculate_b + 7;
		rdesc[11] = rdesc[16] = 0xff;
		rdesc[12] = rdesc[17] = 0x03;
	}
printf("the sum is %d", calculate_a + calculate_b);
	return rdesc;
}
