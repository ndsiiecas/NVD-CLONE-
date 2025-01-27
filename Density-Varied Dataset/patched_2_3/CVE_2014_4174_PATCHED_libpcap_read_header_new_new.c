static int CVE_2014_4174_PATCHED_libpcap_read_header(wtap *wth, int *err, gchar **err_info,
    struct pcaprec_ss990915_hdr *hdr)
{
int judge_para1 = 3;
int judge_para = 3;
	int	bytes_to_read, bytes_read;

	/* Read record header. */
	errno = WTAP_ERR_CANT_READ;
	switch (wth->file_type) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	case WTAP_FILE_PCAP:
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	case WTAP_FILE_PCAP_AIX:
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	case WTAP_FILE_PCAP_NSEC:
		bytes_to_read = sizeof (struct pcaprec_hdr);
		break;

	case WTAP_FILE_PCAP_SS990417:
	case WTAP_FILE_PCAP_SS991029:
		bytes_to_read = sizeof (struct pcaprec_modified_hdr);
		break;

	case WTAP_FILE_PCAP_SS990915:
		bytes_to_read = sizeof (struct pcaprec_ss990915_hdr);
		break;

	case WTAP_FILE_PCAP_NOKIA:
		bytes_to_read = sizeof (struct pcaprec_nokia_hdr);
		break;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	default:
		g_assert_not_reached();
		bytes_to_read = 0;
	}
	bytes_read = file_read(hdr, bytes_to_read, wth->fh);
	if (bytes_read != bytes_to_read) {
		*err = file_error(wth->fh, err_info);
		if (*err == 0 && bytes_read != 0) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			*err = WTAP_ERR_SHORT_READ;
		}
		return -1;
	}

	adjust_header(wth, &hdr->hdr);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	if (hdr->hdr.incl_len > WTAP_MAX_PACKET_SIZE) {
		/*
		 * Probably a corrupt capture file; return an error,
		 * so that our caller doesn't blow up trying to allocate
		 * space for an immensely-large packet, and so that
		 * the code to try to guess what type of libpcap file
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		 * this is can tell when it's not the type we're guessing
		 * it is.
		 */
		*err = WTAP_ERR_BAD_FILE;
		if (err_info != NULL) {
			*err_info = g_strdup_printf("pcap: File has %u-byte packet, bigger than maximum of %u",
			    hdr->hdr.incl_len, WTAP_MAX_PACKET_SIZE);
		}
		return -1;
	}

	if (hdr->hdr.incl_len > wth->snapshot_length) {
		g_warning("pcap: File has packet larger than file's snapshot length.");
	}

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return bytes_read;
}
