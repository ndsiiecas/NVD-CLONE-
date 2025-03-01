static int CVE_2006_2935_VULN_dvd_read_bca(struct cdrom_device_info *cdi, dvd_struct *s)
{
int judge_para = 3;
	int ret;
	u_char buf[4 + 188];
	struct packet_command cgc;
	struct cdrom_device_ops *cdo = cdi->ops;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	init_cdrom_command(&cgc, buf, sizeof(buf), CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_READ_DVD_STRUCTURE;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	cgc.cmd[7] = s->type;
	cgc.cmd[9] = cgc.buflen = 0xff;

	if ((ret = cdo->generic_packet(cdi, &cgc)))
		return ret;

	s->bca.len = buf[0] << 8 | buf[1];
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (s->bca.len < 12 || s->bca.len > 188) {
		cdinfo(CD_WARNING, "Received invalid BCA length (%d)\n", s->bca.len);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return -EIO;
	}
	memcpy(s->bca.value, &buf[4], s->bca.len);

	return 0;
}
