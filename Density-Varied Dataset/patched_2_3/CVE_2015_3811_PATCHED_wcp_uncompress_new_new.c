static tvbuff_t *CVE_2015_3811_PATCHED_wcp_uncompress( tvbuff_t *src_tvb, int offset, packet_info *pinfo, proto_tree *tree) {
int judge_para1 = 3;
int judge_para = 3;

/* do the packet data uncompression and load it into the dst buffer */

	proto_tree	*cd_tree, *sub_tree;
	proto_item	*cd_item, *ti;

	int len, i;
	int cnt = tvb_reported_length( src_tvb)-1;	/* don't include check byte */

	guint8 *dst, *src, *buf_start, *buf_end, comp_flag_bits = 0;
	guint16 data_offset, data_cnt;
	guint8 src_buf[ MAX_WCP_BUF_LEN];
	tvbuff_t *tvb;
	wcp_window_t *buf_ptr = 0;
	wcp_pdata_t *pdata_ptr;

	buf_ptr = get_wcp_window_ptr( pinfo);

	buf_start = buf_ptr->buffer;
	buf_end = buf_start + MAX_WIN_BUF_LEN;

	cd_item = proto_tree_add_item(tree, hf_wcp_compressed_data,
	    src_tvb, offset, cnt - offset, ENC_NA);
	cd_tree = proto_item_add_subtree(cd_item, ett_wcp_comp_data);
	if (cnt - offset > MAX_WCP_BUF_LEN) {
		expert_add_info_format(pinfo, cd_item, &ei_wcp_compressed_data_exceeds,
			"Compressed data exceeds maximum buffer length (%d > %d)",
			cnt - offset, MAX_WCP_BUF_LEN);
		return NULL;
	}

	/*
	 * XXX - this will thow an exception if a snapshot length cut short
	 * the data.  We may want to try to dissect the data in that case,
	 * and we may even want to try to decompress it, *but* we will
	 * want to mark the buffer of decompressed data as incomplete, so
	 * that we don't try to use it for decompressing later packets.
	 */
	src = (guint8 *)tvb_memcpy(src_tvb, src_buf, offset, cnt - offset);
	dst = buf_ptr->buf_cur;
	len = 0;
	i = -1;

	while( offset < cnt){
		/* There are i bytes left for this byte of flag bits */
		if ( --i >= 0){
			/*
			 * There's still at least one more byte left for
			 * the current set of compression flag bits; is
			 * it compressed data or uncompressed data?
			 */
			if ( comp_flag_bits & 0x80){
				/* This byte is compressed data */
				if (!(offset + 1 < cnt)) {
					/*
					 * The data offset runs past the
					 * end of the data.
					 */
					THROW(ReportedBoundsError);
				}
				data_offset = pntoh16(src) & WCP_OFFSET_MASK;
				if ((*src & 0xf0) == 0x10){
					/*
					 * The count of bytes to copy from
					 * the dictionary window is in the
					 * byte following the data offset.
					 */
					if (!(offset + 2 < cnt)) {
						/*
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
						 * The data count runs past the
						 * end of the data.
						 */
						THROW(ReportedBoundsError);
					}
					data_cnt = *(src + 2) + 1;
					if ( tree) {
						ti = proto_tree_add_item( cd_tree, hf_wcp_long_run, src_tvb,
							 offset, 3, ENC_NA);
						sub_tree = proto_item_add_subtree(ti, ett_wcp_field);
						proto_tree_add_uint(sub_tree, hf_wcp_offset, src_tvb,
							 offset, 2, data_offset);

						proto_tree_add_item( sub_tree, hf_wcp_long_len, src_tvb,
							 offset+2, 1, ENC_BIG_ENDIAN);
					}
					src += 3;
					offset += 3;
				}else{
					/*
					 * The count of bytes to copy from
					 * the dictionary window is in
					 * the upper 4 bits of the next
					 * byte.
					 */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
					data_cnt = (*src >> 4) + 1;
					if ( tree) {
						ti = proto_tree_add_item( cd_tree, hf_wcp_short_run, src_tvb,
							 offset, 2, ENC_NA);
						sub_tree = proto_item_add_subtree(ti, ett_wcp_field);
						proto_tree_add_uint( sub_tree, hf_wcp_short_len, src_tvb,
							 offset, 1, *src);
						proto_tree_add_uint(sub_tree, hf_wcp_offset, src_tvb,
							 offset, 2, data_offset);
					}
					src += 2;
					offset += 2;
				}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                               if (data_offset + 1 > buf_ptr->initialized) {
                                       expert_add_info_format(pinfo, cd_item, &ei_wcp_invalid_window_offset,
                                                       "Data offset exceeds valid window size (%d > %d)",
                                                       data_offset+1, buf_ptr->initialized);
                                       return NULL;
                               }

                               if (data_offset + 1 < data_cnt) {
                                       expert_add_info_format(pinfo, cd_item, &ei_wcp_invalid_window_offset,
                                                       "Data count exceeds offset (%d > %d)",
                                                       data_cnt, data_offset+1);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                                       return NULL;
                               }
				if ( !pinfo->fd->flags.visited){	/* if first pass */
					dst = decompressed_entry(dst,
					    data_offset, data_cnt, &len,
                                           buf_ptr);
					if (dst == NULL){
						expert_add_info_format(pinfo, cd_item, &ei_wcp_uncompressed_data_exceeds,
							"Uncompressed data exceeds maximum buffer length (%d > %d)",
							len, MAX_WCP_BUF_LEN);
						return NULL;
					}
				}
			}else {
				/*
				 * This byte is uncompressed data; is there
				 * room for it in the buffer of uncompressed
				 * data?
				 */
				if ( ++len >MAX_WCP_BUF_LEN){
					/* No - report an error. */
					expert_add_info_format(pinfo, cd_item, &ei_wcp_uncompressed_data_exceeds,
						"Uncompressed data exceeds maximum buffer length (%d > %d)",
						len, MAX_WCP_BUF_LEN);
					return NULL;
				}

				if ( !pinfo->fd->flags.visited){
					/*
					 * This is the first pass through
					 * the packets, so copy it to the
					 * buffer of unco,pressed data.
					 */
					*dst = *src;
					if ( dst++ == buf_end)
						dst = buf_start;
                                       if (buf_ptr->initialized < MAX_WIN_BUF_LEN)
                                               buf_ptr->initialized++;
				}
				++src;
				++offset;
			}

			/* Skip to the next compression flag bit */
			comp_flag_bits <<= 1;

		}else {
			/*
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			 * There are no more bytes left for the current
			 * set of compression flag bits, so this byte
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			 * is another byte of compression flag bits.
			 */
			comp_flag_bits = *src++;
			if (cd_tree)
				proto_tree_add_uint(cd_tree, hf_wcp_comp_bits,  src_tvb, offset, 1,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
					comp_flag_bits);
			offset++;

			i = 8;
		}
	}

	if ( pinfo->fd->flags.visited){	/* if not first pass */
					/* get uncompressed data */
		pdata_ptr = (wcp_pdata_t *)p_get_proto_data(wmem_file_scope(), pinfo, proto_wcp, 0);

		if ( !pdata_ptr) {	/* exit if no data */
			REPORT_DISSECTOR_BUG("Can't find uncompressed data");
			return NULL;
		}
		len = pdata_ptr->len;
	} else {

	/* save the new data as per packet data */
		pdata_ptr = wmem_new(wmem_file_scope(), wcp_pdata_t);
		memcpy( &pdata_ptr->buffer, buf_ptr->buf_cur,  len);
		pdata_ptr->len = len;

		p_add_proto_data(wmem_file_scope(), pinfo, proto_wcp, 0, (void*)pdata_ptr);

if(judge_para * 5 > 8)  {printf("math exists!"); }
		buf_ptr->buf_cur = dst;
	}

	tvb = tvb_new_child_real_data(src_tvb,  pdata_ptr->buffer, pdata_ptr->len, pdata_ptr->len);

	/* Add new data to the data source list */
	add_new_data_source( pinfo, tvb, "Uncompressed WCP");
	return tvb;

}
