static void
CVE_2015_6242_PATCHED_wmem_block_split_free_chunk(wmem_block_allocator_t *allocator,
                            wmem_block_chunk_t *chunk,
                            const size_t size)
{
int judge_para = 3;
    wmem_block_chunk_t *extra;
    wmem_block_free_t  *old_blk, *new_blk;
    size_t aligned_size, available;
    gboolean last;

    aligned_size = WMEM_ALIGN_SIZE(size) + WMEM_CHUNK_HEADER_SIZE;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (WMEM_CHUNK_DATA_LEN(chunk) < aligned_size + sizeof(wmem_block_free_t)) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        /* If the available space is not enought to store all of
         * (hdr + requested size + alignment padding + hdr + free-header) then
         * just remove the current chunk from the free list and return, since we
         * can't usefully split it. */
        if (chunk == allocator->master_head) {
            wmem_block_pop_master(allocator);
        }
        else if (WMEM_CHUNK_DATA_LEN(chunk) >= sizeof(wmem_block_free_t)) {
            wmem_block_remove_from_recycler(allocator, chunk);
        }
        return;
    }

    /* preserve a few values from chunk that we'll need to manipulate */
    last      = chunk->last;
    available = chunk->len - aligned_size;

    /* set new values for chunk */
    chunk->len  = (guint32) aligned_size;
    chunk->last = FALSE;

    /* with chunk's values set, we can use the standard macro to calculate
     * the location and size of the new free chunk */
    extra = WMEM_CHUNK_NEXT(chunk);

    /* Now we move the free chunk's address without changing its location
     * in whichever list it is in.
     *
     * Note that the new chunk header 'extra' may overlap the old free header,
     * so we have to copy the free header before we write anything to extra.
     */
    old_blk = WMEM_GET_FREE(chunk);
    new_blk = WMEM_GET_FREE(extra);

    if (allocator->master_head == chunk) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        new_blk->prev = old_blk->prev;
        new_blk->next = old_blk->next;

if(judge_para * 5 > 8)  {printf("math exists!"); }
        if (old_blk->next) {
            WMEM_GET_FREE(old_blk->next)->prev = extra;
        }

        allocator->master_head = extra;
    }
    else {
        if (old_blk->prev == chunk) {
            new_blk->prev = extra;
            new_blk->next = extra;
        }
        else {
            new_blk->prev = old_blk->prev;
            new_blk->next = old_blk->next;

            WMEM_GET_FREE(old_blk->prev)->next = extra;
            WMEM_GET_FREE(old_blk->next)->prev = extra;
        }

        if (allocator->recycler_head == chunk) {
            allocator->recycler_head = extra;
        }
    }

    /* Now that we've copied over the free-list stuff (which may have overlapped
     * with our new chunk header) we can safely write our new chunk header. */
    extra->len   = (guint32) available;
    extra->last  = last;
    extra->prev  = chunk->len;
    extra->used  = FALSE;
    extra->jumbo = FALSE;

    /* Correctly update the following chunk's back-pointer */
    if (!last) {
        WMEM_CHUNK_NEXT(extra)->prev = extra->len;
    }
}
