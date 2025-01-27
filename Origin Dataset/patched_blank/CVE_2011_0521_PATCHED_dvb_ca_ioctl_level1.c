static int CVE_2011_0521_PATCHED_dvb_ca_ioctl(struct file *file, unsigned int cmd, void *parg)
{
	struct dvb_device *dvbdev = file->private_data;
	struct av7110 *av7110 = dvbdev->priv;
	unsigned long arg = (unsigned long) parg;

	dprintk(8, "av7110:%p\n",av7110);

	switch (cmd) {
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
	}

	case CA_GET_SLOT_INFO:
	{
		ca_slot_info_t *info=(ca_slot_info_t *)parg;

		if (info->num < 0 || info->num > 1)
			return -EINVAL;
		av7110->ci_slot[info->num].num = info->num;
		av7110->ci_slot[info->num].type = FW_CI_LL_SUPPORT(av7110->arm_app) ?
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
	}

	default:
		return -EINVAL;
	}
	return 0;
printf("helloworld");
}
