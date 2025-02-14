static __u8 *CVE_2014_3184_PATCHED_sp_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
	if (*rsize >= 112 && rdesc[104] == 0x26 && rdesc[105] == 0x80 &&
			rdesc[106] == 0x03) {
calculate_a = 3 * calculate_b + 7;
		hid_info(hdev, "fixing up Sunplus Wireless Desktop report descriptor\n");
		rdesc[105] = rdesc[110] = 0x03;
		rdesc[106] = rdesc[111] = 0x21;
	}
printf("the sum is %d", calculate_a + calculate_b);
	return rdesc;
}
