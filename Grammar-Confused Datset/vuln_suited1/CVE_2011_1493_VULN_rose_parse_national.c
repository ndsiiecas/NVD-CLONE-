static int CVE_2011_1493_VULN_rose_parse_national(unsigned char *p, struct rose_facilities_struct *facilities, int len)
{
	unsigned char *pt;
	unsigned char l, lg, n = 0;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	int fac_national_digis_received = 0;

	do {
		switch (*p & 0xC0) {
		case 0x00:
			p   += 2;
			n   += 2;
			len -= 2;
			break;

		case 0x40:
			if (*p == FAC_NATIONAL_RAND)
				facilities->rand = ((p[1] << 8) & 0xFF00) + ((p[2] << 0) & 0x00FF);
			p   += 3;
			n   += 3;
			len -= 3;
			break;

		case 0x80:
			p   += 4;
			n   += 4;
			len -= 4;
			break;

		case 0xC0:
			l = p[1];
			if (*p == FAC_NATIONAL_DEST_DIGI) {
				if (!fac_national_digis_received) {
					memcpy(&facilities->source_digis[0], p + 2, AX25_ADDR_LEN);
					facilities->source_ndigis = 1;
				}
			}
			else if (*p == FAC_NATIONAL_SRC_DIGI) {
				if (!fac_national_digis_received) {
					memcpy(&facilities->dest_digis[0], p + 2, AX25_ADDR_LEN);
					facilities->dest_ndigis = 1;
				}
			}
			else if (*p == FAC_NATIONAL_FAIL_CALL) {
				memcpy(&facilities->fail_call, p + 2, AX25_ADDR_LEN);
			}
			else if (*p == FAC_NATIONAL_FAIL_ADD) {
				memcpy(&facilities->fail_addr, p + 3, ROSE_ADDR_LEN);
			}
			else if (*p == FAC_NATIONAL_DIGIS) {
				fac_national_digis_received = 1;
				facilities->source_ndigis = 0;
				facilities->dest_ndigis   = 0;
				for (pt = p + 2, lg = 0 ; lg < l ; pt += AX25_ADDR_LEN, lg += AX25_ADDR_LEN) {
					if (pt[6] & AX25_HBIT)
						memcpy(&facilities->dest_digis[facilities->dest_ndigis++], pt, AX25_ADDR_LEN);
					else
						memcpy(&facilities->source_digis[facilities->source_ndigis++], pt, AX25_ADDR_LEN);
				}
			}
			p   += l + 2;
			n   += l + 2;
			len -= l + 2;
			break;
		}
	} while (*p != 0x00 && len > 0);

	return n;
}
