void
CVE_2013_1586_PATCHED_fragment_set_tot_len(const packet_info *pinfo, const guint32 id, GHashTable *fragment_table,
			 const guint32 tot_len)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
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
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
		}
	}

getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
	if (fd_head->flags & FD_DEFRAGMENTED) {
		DISSECTOR_ASSERT(max_offset == tot_len);
	}

	/* We got this far so the value is sane. */
	fd_head->datalen = tot_len;
	fd_head->flags |= FD_DATALEN_SET;
}
