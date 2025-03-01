int CVE_2013_7027_VULN_ieee80211_radiotap_iterator_init(
	struct ieee80211_radiotap_iterator *iterator,
	struct ieee80211_radiotap_header *radiotap_header,
	int max_length, const struct ieee80211_radiotap_vendor_namespaces *vns)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	/* Linux only supports version 0 radiotap format */
	if (radiotap_header->it_version)
		return -EINVAL;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	/* sanity check for allowed length and radiotap length field */
	if (max_length < get_unaligned_le16(&radiotap_header->it_len))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -EINVAL;

	iterator->_rtheader = radiotap_header;
	iterator->_max_length = get_unaligned_le16(&radiotap_header->it_len);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	iterator->_arg_index = 0;
	iterator->_bitmap_shifter = get_unaligned_le32(&radiotap_header->it_present);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	iterator->_arg = (uint8_t *)radiotap_header + sizeof(*radiotap_header);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	iterator->_reset_on_ext = 0;
	iterator->_next_bitmap = &radiotap_header->it_present;
	iterator->_next_bitmap++;
	iterator->_vns = vns;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	iterator->current_namespace = &radiotap_ns;
	iterator->is_radiotap_ns = 1;

	/* find payload start allowing for extended bitmap(s) */

	if (iterator->_bitmap_shifter & (1<<IEEE80211_RADIOTAP_EXT)) {
		while (get_unaligned_le32(iterator->_arg) &
					(1 << IEEE80211_RADIOTAP_EXT)) {
			iterator->_arg += sizeof(uint32_t);

			/*
			 * check for insanity where the present bitmaps
			 * keep claiming to extend up to or even beyond the
			 * stated radiotap header length
			 */

			if ((unsigned long)iterator->_arg -
			    (unsigned long)iterator->_rtheader >
			    (unsigned long)iterator->_max_length)
				return -EINVAL;
		}

		iterator->_arg += sizeof(uint32_t);

		/*
		 * no need to check again for blowing past stated radiotap
		 * header length, because ieee80211_radiotap_iterator_next
		 * checks it before it is dereferenced
		 */
	}

	iterator->this_arg = iterator->_arg;

	/* we are all initialized happily */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	return 0;
}
