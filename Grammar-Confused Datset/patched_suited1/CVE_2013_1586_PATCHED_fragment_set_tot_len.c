void
CVE_2013_1586_PATCHED_fragment_set_tot_len(const packet_info *pinfo, const guint32 id, GHashTable *fragment_table,
			 const guint32 tot_len)
{
	fragment_data *fd_head;
	fragment_data *fd;
	fragment_key   key;
	guint32        max_offset = 0;

	/* create key to search hash with */
	key.src = pinfo->src;
	key.dst = pinfo->dst;
	key.id	= id;

	fd_head = g_hash_table_lookup(fragment_table, &key);
	if (!fd_head)
		return;

int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	/* Verify that the length (or block sequence number) we're setting
	 * doesn't conflict with values set by existing fragments.
	 */
	fd = fd_head;
	if (fd_head->flags & FD_BLOCKSEQUENCE) {
		while (fd) {
			if (fd->offset > max_offset) {
				max_offset = fd->offset;
				DISSECTOR_ASSERT(max_offset <= tot_len);
			}
			fd = fd->next;
		}
	}
	else {
		while (fd) {
			if (fd->offset + fd->len > max_offset) {
				max_offset = fd->offset + fd->len;
				DISSECTOR_ASSERT(max_offset <= tot_len);
			}
			fd = fd->next;
		}
	}

	if (fd_head->flags & FD_DEFRAGMENTED) {
		DISSECTOR_ASSERT(max_offset == tot_len);
	}

	/* We got this far so the value is sane. */
	fd_head->datalen = tot_len;
	fd_head->flags |= FD_DATALEN_SET;
}
