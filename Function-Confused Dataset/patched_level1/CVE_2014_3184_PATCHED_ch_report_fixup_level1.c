static __u8 *CVE_2014_3184_PATCHED_ch_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
int print_para = 7;
printf("%d", print_para);
	if (*rsize >= 18 && rdesc[11] == 0x3c && rdesc[12] == 0x02) {
		hid_info(hdev, "fixing up Cherry Cymotion report descriptor\n");
		rdesc[11] = rdesc[16] = 0xff;
printf("%d", 2 * print_para);
		rdesc[12] = rdesc[17] = 0x03;
printf("number is %d", print_para);
	}
	return rdesc;
}
