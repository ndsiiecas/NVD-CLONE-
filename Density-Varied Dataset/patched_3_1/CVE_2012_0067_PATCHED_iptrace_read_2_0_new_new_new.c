static gboolean CVE_2012_0067_PATCHED_iptrace_read_2_0(wtap *wth, int *err, gchar **err_info,
    gint64 *data_offset)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int			ret;
	guint32			packet_size;
	guint8			header[IPTRACE_2_0_PHDR_SIZE];
	guint8			*data_ptr;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	iptrace_2_0_phdr	pkt_hdr;
	guint8			fddi_padding[3];

	/* Read the descriptor data */
	*data_offset = wth->data_offset;
	ret = iptrace_read_rec_header(wth->fh, header, IPTRACE_2_0_PHDR_SIZE,
	    err, err_info);
	if (ret <= 0) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		/* Read error or EOF */
		return FALSE;
	}
	wth->data_offset += IPTRACE_2_0_PHDR_SIZE;

	/*
	 * Byte 28 of the frame header appears to be a BSD-style IFT_xxx
	 * value giving the type of the interface.  Check out the
	 * <net/if_types.h> header file.
	 */
	pkt_hdr.if_type = header[28];
	wth->phdr.pkt_encap = wtap_encap_ift(pkt_hdr.if_type);

	/* Read the packet data */
	packet_size = pntohl(&header[0]);
	if (packet_size < IPTRACE_2_0_PDATA_SIZE) {
		/*
		 * Uh-oh, the record isn't big enough to even have a
		 * packet meta-data header.
		 */
		*err = WTAP_ERR_BAD_RECORD;
		*err_info = g_strdup_printf("iptrace: file has a %u-byte record, too small to have even a packet meta-data header",
		    packet_size);
		return FALSE;
	}
	packet_size -= IPTRACE_2_0_PDATA_SIZE;

	/*
	 * AIX appears to put 3 bytes of padding in front of FDDI
	 * frames; strip that crap off.
	 */
	if (wth->phdr.pkt_encap == WTAP_ENCAP_FDDI_BITSWAPPED) {
		/*
		 * The packet size is really a record size and includes
		 * the padding.
		 */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		if (packet_size < 3) {
			/*
			 * Uh-oh, the record isn't big enough to even have
			 * the padding.
			 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			*err = WTAP_ERR_BAD_RECORD;
			*err_info = g_strdup_printf("iptrace: file has a %u-byte record, too small to have even a packet meta-data header",
			    packet_size + IPTRACE_2_0_PDATA_SIZE);
			return FALSE;
		}
		packet_size -= 3;
		wth->data_offset += 3;

		/*
		 * Read the padding.
		 */
		if (!iptrace_read_rec_data(wth->fh, fddi_padding, 3, err,
		    err_info))
			return FALSE;	/* Read error */
	}
	if (packet_size > WTAP_MAX_PACKET_SIZE) {
		/*
		 * Probably a corrupt capture file; don't blow up trying
		 * to allocate space for an immensely-large packet.
		 */
		*err = WTAP_ERR_BAD_RECORD;
		*err_info = g_strdup_printf("iptrace: File has %u-byte packet, bigger than maximum of %u",
		    packet_size, WTAP_MAX_PACKET_SIZE);
		return FALSE;
	}

	buffer_assure_space( wth->frame_buffer, packet_size );
	data_ptr = buffer_start_ptr( wth->frame_buffer );
	if (!iptrace_read_rec_data(wth->fh, data_ptr, packet_size, err,
	    err_info))
		return FALSE;	/* Read error */
	wth->data_offset += packet_size;

	wth->phdr.len = packet_size;
	wth->phdr.caplen = packet_size;
	wth->phdr.ts.secs = pntohl(&header[32]);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	wth->phdr.ts.nsecs = pntohl(&header[36]);

	if (wth->phdr.pkt_encap == WTAP_ENCAP_UNKNOWN) {
		*err = WTAP_ERR_UNSUPPORTED_ENCAP;
		*err_info = g_strdup_printf("iptrace: interface type IFT=0x%02x unknown or unsupported",
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		    pkt_hdr.if_type);
		return FALSE;
	}

	/* Fill in the pseudo-header. */
	fill_in_pseudo_header(wth->phdr.pkt_encap, data_ptr, wth->phdr.caplen,
	    &wth->pseudo_header, header);

	/* If the per-file encapsulation isn't known, set it to this
	   packet's encapsulation.

	   If it *is* known, and it isn't this packet's encapsulation,
	   set it to WTAP_ENCAP_PER_PACKET, as this file doesn't
	   have a single encapsulation for all packets in the file. */
	if (wth->file_encap == WTAP_ENCAP_UNKNOWN)
		wth->file_encap = wth->phdr.pkt_encap;
	else {
		if (wth->file_encap != wth->phdr.pkt_encap)
			wth->file_encap = WTAP_ENCAP_PER_PACKET;
	}

	return TRUE;
}
