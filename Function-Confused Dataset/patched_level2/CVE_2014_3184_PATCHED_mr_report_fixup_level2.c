static __u8 *CVE_2014_3184_PATCHED_mr_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
int judge_para = 3;
	if (*rsize >= 31 && rdesc[29] == 0x05 && rdesc[30] == 0x09) {
		hid_info(hdev, "fixing up button/consumer in HID report descriptor\n");
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		rdesc[30] = 0x0c;
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
	return rdesc;
}
