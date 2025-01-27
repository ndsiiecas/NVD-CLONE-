static int CVE_2011_0521_VULN_dvb_ca_ioctl(struct file *file, unsigned int cmd, void *parg)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct dvb_device *dvbdev = file->private_data;
	struct av7110 *av7110 = dvbdev->priv;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long arg = (unsigned long) parg;

	dprintk(8, "av7110:%p\n",av7110);

	switch (cmd) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	case CA_RESET:
		return ci_ll_reset(&av7110->ci_wbuffer, file, arg, &av7110->ci_slot[0]);
		break;
	case CA_GET_CAP:
	{
		ca_caps_t cap;

		cap.slot_num = 2;
		cap.slot_type = (FW_CI_LL_SUPPORT(av7110->arm_app) ?
				 CA_CI_LINK : CA_CI) | CA_DESCR;
		cap.descr_num = 16;
		cap.descr_type = CA_ECD;
		memcpy(parg, &cap, sizeof(cap));
		break;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	}

	case CA_GET_SLOT_INFO:
	{
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		ca_slot_info_t *info=(ca_slot_info_t *)parg;

		if (info->num > 1)
			return -EINVAL;
		av7110->ci_slot[info->num].num = info->num;
		av7110->ci_slot[info->num].type = FW_CI_LL_SUPPORT(av7110->arm_app) ?
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
							CA_CI_LINK : CA_CI;
		memcpy(info, &av7110->ci_slot[info->num], sizeof(ca_slot_info_t));
		break;
	}

	case CA_GET_MSG:
		break;

	case CA_SEND_MSG:
		break;

	case CA_GET_DESCR_INFO:
	{
		ca_descr_info_t info;

		info.num = 16;
		info.type = CA_ECD;
		memcpy(parg, &info, sizeof (info));
		break;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	case CA_SET_DESCR:
	{
		ca_descr_t *descr = (ca_descr_t*) parg;

		if (descr->index >= 16)
			return -EINVAL;
		if (descr->parity > 1)
			return -EINVAL;
		av7110_fw_cmd(av7110, COMTYPE_PIDFILTER, SetDescr, 5,
			      (descr->index<<8)|descr->parity,
			      (descr->cw[0]<<8)|descr->cw[1],
			      (descr->cw[2]<<8)|descr->cw[3],
			      (descr->cw[4]<<8)|descr->cw[5],
			      (descr->cw[6]<<8)|descr->cw[7]);
		break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	default:
		return -EINVAL;
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	return 0;
}
