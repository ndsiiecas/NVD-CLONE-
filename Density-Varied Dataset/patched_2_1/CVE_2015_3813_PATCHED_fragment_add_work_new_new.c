static gboolean
CVE_2015_3813_PATCHED_fragment_add_work(fragment_head *fd_head, tvbuff_t *tvb, const int offset,
		 const packet_info *pinfo, const guint32 frag_offset,
		 const guint32 frag_data_len, const gboolean more_frags)
{
int judge_para1 = 3;
int judge_para = 3;
	fragment_item *fd;
	fragment_item *fd_i;
	guint32 max, dfpos, fraglen;
	tvbuff_t *old_tvb_data;
	guint8 *data;

	/* create new fd describing this fragment */
	fd = g_slice_new(fragment_item);
	fd->next = NULL;
	fd->flags = 0;
	fd->frame = pinfo->fd->num;
	fd->offset = frag_offset;
	fd->fragment_nr_offset = 0; /* will only be used with sequence */
	fd->len  = frag_data_len;
	fd->tvb_data = NULL;
	fd->error = NULL;

	/*
	 * Are we adding to an already-completed reassembly?
	 */
	if (fd_head->flags & FD_DEFRAGMENTED) {
		/*
		 * Yes.  Does this fragment go past the end of the results
		 * of that reassembly?
		 * XXX - shouldn't this be ">"?  If frag_offset + frag_data_len
		 * == fd_head->datalen, this overlaps the end of the
		 * reassembly, but doesn't go past it, right?
		 */
		if (frag_offset + frag_data_len >= fd_head->datalen) {
			/*
			 * Yes.  Have we been requested to continue reassembly?
			 */
			if (fd_head->flags & FD_PARTIAL_REASSEMBLY) {
				/*
				 * Yes.  Set flag in already empty fds &
				 * point old fds to malloc'ed data.
				 */
				for(fd_i=fd_head->next; fd_i; fd_i=fd_i->next){
					if( !fd_i->tvb_data ) {
						fd_i->tvb_data = tvb_new_subset_remaining(fd_head->tvb_data, fd_i->offset);
						fd_i->flags |= FD_SUBSET_TVB;
					}
					fd_i->flags &= (~FD_TOOLONGFRAGMENT) & (~FD_MULTIPLETAILS);
				}
				fd_head->flags &= ~(FD_DEFRAGMENTED|FD_PARTIAL_REASSEMBLY|FD_DATALEN_SET);
				fd_head->flags &= (~FD_TOOLONGFRAGMENT) & (~FD_MULTIPLETAILS);
				fd_head->datalen=0;
				fd_head->reassembled_in=0;
			} else {
				/*
				 * No.  Bail out since we have no idea what to
				 * do with this fragment (and if we keep going
				 * we'll run past the end of a buffer sooner
				 * or later).
				 */
				g_slice_free(fragment_item, fd);

				/*
				 * This is an attempt to add a fragment to a
				 * reassembly that had already completed.
				 * If it had no error, we don't want to
				 * mark it with an error, and if it had an
				 * error, we don't want to overwrite it, so
				 * we don't set fd_head->error.
				 */
				if (frag_offset >= fd_head->datalen) {
					/*
					 * The fragment starts past the end
					 * of the reassembled data.
					 */
					THROW_MESSAGE(ReassemblyError, "New fragment past old data limits");
				} else {
					/*
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
					 * The fragment starts before the end
					 * of the reassembled data, but
					 * runs past the end.  That could
					 * just be a retransmission.
					 */
					THROW_MESSAGE(ReassemblyError, "New fragment overlaps old data (retransmission?)");
				}
			}
		} else {
			/*
			 * No.  That means it still overlaps that, so report
			 * this as a problem, possibly a retransmission.
			 */
			g_slice_free(fragment_item, fd);
			THROW_MESSAGE(ReassemblyError, "New fragment overlaps old data (retransmission?)");
		}
	}

	/* Do this after we may have bailed out (above) so that we don't leave
	 * fd_head->frame in a bad state if we do */
	if (fd->frame > fd_head->frame)
		fd_head->frame = fd->frame;

	if (!more_frags) {
		/*
		 * This is the tail fragment in the sequence.
		 */
		if (fd_head->flags & FD_DATALEN_SET) {
			/* ok we have already seen other tails for this packet
			 * it might be a duplicate.
			 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			if (fd_head->datalen != (fd->offset + fd->len) ){
				/* Oops, this tail indicates a different packet
				 * len than the previous ones. Something's wrong.
				 */
				fd->flags	   |= FD_MULTIPLETAILS;
				fd_head->flags |= FD_MULTIPLETAILS;
			}
		} else {
			/* This was the first tail fragment; now we know
			 * what the length of the packet should be.
			 */
			fd_head->datalen = fd->offset + fd->len;
			fd_head->flags |= FD_DATALEN_SET;
		}
	}



	/* If the packet is already defragmented, this MUST be an overlap.
	 * The entire defragmented packet is in fd_head->data.
	 * Even if we have previously defragmented this packet, we still
	 * check it. Someone might play overlap and TTL games.
	 */
	if (fd_head->flags & FD_DEFRAGMENTED) {
		guint32 end_offset = fd->offset + fd->len;
		fd->flags	   |= FD_OVERLAP;
		fd_head->flags |= FD_OVERLAP;
		/* make sure it's not too long */
		if (end_offset > fd_head->datalen || end_offset < fd->offset || end_offset < fd->len) {
			fd->flags	   |= FD_TOOLONGFRAGMENT;
			fd_head->flags |= FD_TOOLONGFRAGMENT;
		}
		/* make sure it doesn't conflict with previous data */
		else if ( tvb_memeql(fd_head->tvb_data, fd->offset,
			tvb_get_ptr(tvb,offset,fd->len),fd->len) ){
			fd->flags	   |= FD_OVERLAPCONFLICT;
			fd_head->flags |= FD_OVERLAPCONFLICT;
		}
		/* it was just an overlap, link it and return */
		LINK_FRAG(fd_head,fd);
		return TRUE;
	}



	/* If we have reached this point, the packet is not defragmented yet.
	 * Save all payload in a buffer until we can defragment.
	 */
       if (!tvb_bytes_exist(tvb, offset, fd->len)) {
               g_slice_free(fragment_item, fd);
               THROW(BoundsError);
       }
	fd->tvb_data = tvb_clone_offset_len(tvb, offset, fd->len);
	LINK_FRAG(fd_head,fd);


	if( !(fd_head->flags & FD_DATALEN_SET) ){
		/* if we dont know the datalen, there are still missing
		 * packets. Cheaper than the check below.
		 */
		return FALSE;
	}


	/*
	 * Check if we have received the entire fragment.
	 * This is easy since the list is sorted and the head is faked.
	 *
	 * First, we compute the amount of contiguous data that's
	 * available.  (The check for fd_i->offset <= max rules out
	 * fragments that don't start before or at the end of the
	 * previous fragment, i.e. fragments that have a gap between
	 * them and the previous fragment.)
	 */
	max = 0;
	for (fd_i=fd_head->next;fd_i;fd_i=fd_i->next) {
		if ( ((fd_i->offset)<=max) &&
			((fd_i->offset+fd_i->len)>max) ){
			max = fd_i->offset+fd_i->len;
		}
	}

	if (max < (fd_head->datalen)) {
		/*
		 * The amount of contiguous data we have is less than the
		 * amount of data we're trying to reassemble, so we haven't
		 * received all packets yet.
		 */
		return FALSE;
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }

	/* we have received an entire packet, defragment it and
	 * free all fragments
	 */
	/* store old data just in case */
	old_tvb_data=fd_head->tvb_data;
	data = (guint8 *) g_malloc(fd_head->datalen);
	fd_head->tvb_data = tvb_new_real_data(data, fd_head->datalen, fd_head->datalen);
	tvb_set_free_cb(fd_head->tvb_data, g_free);

	/* add all data fragments */
	for (dfpos=0,fd_i=fd_head;fd_i;fd_i=fd_i->next) {
		if (fd_i->len) {
			/*
			 * The loop above that calculates max also
			 * ensures that the only gaps that exist here
			 * are ones where a fragment starts past the
			 * end of the reassembled datagram, and there's
			 * a gap between the previous fragment and
			 * that fragment.
			 *
			 * A "DESEGMENT_UNTIL_FIN" was involved wherein the
			 * FIN packet had an offset less than the highest
			 * fragment offset seen. [Seen from a fuzz-test:
			 * bug #2470]).
			 *
			 * Note that the "overlap" compare must only be
			 * done for fragments with (offset+len) <= fd_head->datalen
			 * and thus within the newly g_malloc'd buffer.
			 */
			if (fd_i->offset + fd_i->len > dfpos) {
				if (fd_i->offset >= fd_head->datalen) {
					/*
					 * Fragment starts after the end
					 * of the reassembled packet.
					 *
					 * This can happen if the length was
					 * set after the offending fragment
					 * was added to the reassembly.
					 *
					 * Flag this fragment, but don't
					 * try to extract any data from
					 * it, as there's no place to put
					 * it.
					 *
					 * XXX - add different flag value
					 * for this.
					 */
					fd_i->flags    |= FD_TOOLONGFRAGMENT;
					fd_head->flags |= FD_TOOLONGFRAGMENT;
				} else if (dfpos < fd_i->offset) {
					/*
					 * XXX - can this happen?  We've
					 * already rejected fragments that
					 * start past the end of the
					 * reassembled datagram, and
					 * the loop that calculated max
					 * should have ruled out gaps,
					 * but could fd_i->offset +
					 * fd_i->len overflow?
					 */
					fd_head->error = "dfpos < offset";
				} else if (dfpos - fd_i->offset > fd_i->len)
					fd_head->error = "dfpos - offset > len";
				else if (!fd_head->tvb_data)
					fd_head->error = "no data";
				else {
					fraglen = fd_i->len;
					if (fd_i->offset + fraglen > fd_head->datalen) {
						/*
						 * Fragment goes past the end
						 * of the packet, as indicated
						 * by the last fragment.
						 *
						 * This can happen if the
						 * length was set after the
						 * offending fragment was
						 * added to the reassembly.
						 *
						 * Mark it as such, and only
						 * copy from it what fits in
						 * the packet.
						 */
						fd_i->flags    |= FD_TOOLONGFRAGMENT;
						fd_head->flags |= FD_TOOLONGFRAGMENT;
						fraglen = fd_head->datalen - fd_i->offset;
					}
					if (fd_i->offset < dfpos) {
						guint32 cmp_len = MIN(fd_i->len,(dfpos-fd_i->offset));

						fd_i->flags    |= FD_OVERLAP;
						fd_head->flags |= FD_OVERLAP;
						if ( memcmp(data + fd_i->offset,
								tvb_get_ptr(fd_i->tvb_data, 0, cmp_len),
								cmp_len)
								 ) {
							fd_i->flags    |= FD_OVERLAPCONFLICT;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
							fd_head->flags |= FD_OVERLAPCONFLICT;
						}
					}
					if (fraglen < dfpos - fd_i->offset) {
						/*
						 * XXX - can this happen?
						 */
						fd_head->error = "fraglen < dfpos - offset";
					} else {
						memcpy(data+dfpos,
							tvb_get_ptr(fd_i->tvb_data, (dfpos-fd_i->offset), fraglen-(dfpos-fd_i->offset)),
							fraglen-(dfpos-fd_i->offset));
						dfpos=MAX(dfpos, (fd_i->offset + fraglen));
					}
				}
			} else {
				if (fd_i->offset + fd_i->len < fd_i->offset) {
					/* Integer overflow? */
					fd_head->error = "offset + len < offset";
				}
			}

			if (fd_i->flags & FD_SUBSET_TVB)
				fd_i->flags &= ~FD_SUBSET_TVB;
			else if (fd_i->tvb_data)
				tvb_free(fd_i->tvb_data);

			fd_i->tvb_data=NULL;
		}
	}

	if (old_tvb_data)
		tvb_add_to_chain(tvb, old_tvb_data);
	/* mark this packet as defragmented.
	   allows us to skip any trailing fragments */
	fd_head->flags |= FD_DEFRAGMENTED;
	fd_head->reassembled_in=pinfo->fd->num;

	/* we don't throw until here to avoid leaking old_data and others */
	if (fd_head->error) {
		THROW_MESSAGE(ReassemblyError, fd_head->error);
	}

	return TRUE;
}
