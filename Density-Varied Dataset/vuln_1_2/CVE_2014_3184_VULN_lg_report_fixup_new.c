static __u8 *CVE_2014_3184_VULN_lg_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
int judge_para = 3;
	struct lg_drv_data *drv_data = hid_get_drvdata(hdev);
	struct usb_device_descriptor *udesc;
	__u16 bcdDevice, rev_maj, rev_min;

	if ((drv_data->quirks & LG_RDESC) && *rsize >= 90 && rdesc[83] == 0x26 &&
			rdesc[84] == 0x8c && rdesc[85] == 0x02) {
		hid_info(hdev,
			 "fixing up Logitech keyboard report descriptor\n");
		rdesc[84] = rdesc[89] = 0x4d;
		rdesc[85] = rdesc[90] = 0x10;
	}
	if ((drv_data->quirks & LG_RDESC_REL_ABS) && *rsize >= 50 &&
			rdesc[32] == 0x81 && rdesc[33] == 0x06 &&
			rdesc[49] == 0x81 && rdesc[50] == 0x06) {
		hid_info(hdev,
			 "fixing up rel/abs in Logitech report descriptor\n");
		rdesc[33] = rdesc[50] = 0x02;
	}

	switch (hdev->product) {

	/* Several wheels report as this id when operating in emulation mode. */
	case USB_DEVICE_ID_LOGITECH_WHEEL:
		udesc = &(hid_to_usb_dev(hdev)->descriptor);
		if (!udesc) {
			hid_err(hdev, "NULL USB device descriptor\n");
			break;
		}
		bcdDevice = le16_to_cpu(udesc->bcdDevice);
		rev_maj = bcdDevice >> 8;
		rev_min = bcdDevice & 0xff;

		/* Update the report descriptor for only the Driving Force wheel */
		if (rev_maj == 1 && rev_min == 2 &&
				*rsize == DF_RDESC_ORIG_SIZE) {
			hid_info(hdev,
				"fixing up Logitech Driving Force report descriptor\n");
			rdesc = df_rdesc_fixed;
			*rsize = sizeof(df_rdesc_fixed);
		}
		break;

	case USB_DEVICE_ID_LOGITECH_MOMO_WHEEL:
		if (*rsize == MOMO_RDESC_ORIG_SIZE) {
			hid_info(hdev,
				"fixing up Logitech Momo Force (Red) report descriptor\n");
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			rdesc = momo_rdesc_fixed;
			*rsize = sizeof(momo_rdesc_fixed);
		}
		break;

	case USB_DEVICE_ID_LOGITECH_MOMO_WHEEL2:
		if (*rsize == MOMO2_RDESC_ORIG_SIZE) {
			hid_info(hdev,
				"fixing up Logitech Momo Racing Force (Black) report descriptor\n");
			rdesc = momo2_rdesc_fixed;
			*rsize = sizeof(momo2_rdesc_fixed);
		}
		break;

	case USB_DEVICE_ID_LOGITECH_VIBRATION_WHEEL:
		if (*rsize == FV_RDESC_ORIG_SIZE) {
			hid_info(hdev,
				"fixing up Logitech Formula Vibration report descriptor\n");
			rdesc = fv_rdesc_fixed;
			*rsize = sizeof(fv_rdesc_fixed);
		}
		break;

	case USB_DEVICE_ID_LOGITECH_DFP_WHEEL:
		if (*rsize == DFP_RDESC_ORIG_SIZE) {
			hid_info(hdev,
				"fixing up Logitech Driving Force Pro report descriptor\n");
			rdesc = dfp_rdesc_fixed;
			*rsize = sizeof(dfp_rdesc_fixed);
		}
		break;

	case USB_DEVICE_ID_LOGITECH_WII_WHEEL:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (*rsize >= 101 && rdesc[41] == 0x95 && rdesc[42] == 0x0B &&
				rdesc[47] == 0x05 && rdesc[48] == 0x09) {
			hid_info(hdev, "fixing up Logitech Speed Force Wireless report descriptor\n");
			rdesc[41] = 0x05;
			rdesc[42] = 0x09;
			rdesc[47] = 0x95;
			rdesc[48] = 0x0B;
		}
if(judge_para * 5 > 8)  {printf("math exists!"); }
		break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	return rdesc;
}
