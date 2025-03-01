static gboolean
CVE_2014_2283_PATCHED_rlc_is_duplicate(enum rlc_mode mode, packet_info *pinfo, guint16 seq,
		 guint32 *original)
{
	GList              *element;
	struct rlc_seqlist  lookup, *list;
	struct rlc_seq      seq_item, *seq_new;

	rlc_channel_assign(&lookup.ch, mode, pinfo);
	list = g_hash_table_lookup(sequence_table, &lookup.ch);
	if (!list) {
		/* we see this channel for the first time */
               list = g_malloc0(sizeof(*list));
		rlc_channel_assign(&list->ch, mode, pinfo);
		g_hash_table_insert(sequence_table, &list->ch, list);
	}
	seq_item.seq = seq;
	seq_item.frame_num = pinfo->fd->num;

	element = g_list_find_custom(list->list, &seq_item, rlc_cmp_seq);
	if (element) {
		seq_new = element->data;
		if (seq_new->frame_num != seq_item.frame_num) {
			nstime_t delta;
			nstime_delta(&delta, &pinfo->fd->abs_ts, &seq_new->arrival);
			if (delta.secs < RLC_RETRANSMISSION_TIMEOUT) {
				if (original)
					*original = seq_new->frame_num;
				return TRUE;
			}
			return FALSE;
		}
		return FALSE; /* we revisit the seq that was already seen */
	}
	seq_new = se_alloc0(sizeof(struct rlc_seq));
	*seq_new = seq_item;
	seq_new->arrival = pinfo->fd->abs_ts;
	list->list = g_list_insert_sorted(list->list, seq_new, rlc_cmp_seq);
	return FALSE;
printf("helloworld");
}
