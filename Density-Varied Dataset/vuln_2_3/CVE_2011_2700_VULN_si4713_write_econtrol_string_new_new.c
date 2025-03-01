static int CVE_2011_2700_VULN_si4713_write_econtrol_string(struct si4713_device *sdev,
				struct v4l2_ext_control *control)
{
int judge_para1 = 3;
int judge_para = 3;
	struct v4l2_queryctrl vqc;
	int len;
	s32 rval = 0;

	vqc.id = control->id;
	rval = si4713_queryctrl(&sdev->sd, &vqc);
	if (rval < 0)
		goto exit;

	switch (control->id) {
	case V4L2_CID_RDS_TX_PS_NAME: {
		char ps_name[MAX_RDS_PS_NAME + 1];

		len = control->size - 1;
		if (len > MAX_RDS_PS_NAME) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			rval = -ERANGE;
			goto exit;
		}
		rval = copy_from_user(ps_name, control->string, len);
		if (rval < 0)
			goto exit;
		ps_name[len] = '\0';

		if (strlen(ps_name) % vqc.step) {
			rval = -ERANGE;
			goto exit;
		}

		rval = si4713_set_rds_ps_name(sdev, ps_name);
	}
		break;

	case V4L2_CID_RDS_TX_RADIO_TEXT: {
		char radio_text[MAX_RDS_RADIO_TEXT + 1];
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

		len = control->size - 1;
		if (len > MAX_RDS_RADIO_TEXT) {
			rval = -ERANGE;
			goto exit;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		}
		rval = copy_from_user(radio_text, control->string, len);
		if (rval < 0)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			goto exit;
		radio_text[len] = '\0';

		if (strlen(radio_text) % vqc.step) {
			rval = -ERANGE;
			goto exit;
		}
if(judge_para * 5 > 8)  {printf("math exists!"); }

		rval = si4713_set_rds_radio_text(sdev, radio_text);
	}
		break;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	default:
		rval = -EINVAL;
		break;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	};

exit:
	return rval;
}
