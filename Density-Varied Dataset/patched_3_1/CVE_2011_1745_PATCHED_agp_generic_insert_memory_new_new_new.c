int CVE_2011_1745_PATCHED_agp_generic_insert_memory(struct agp_memory * mem, off_t pg_start, int type)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int num_entries;
	size_t i;
	off_t j;
	void *temp;
	struct agp_bridge_data *bridge;
	int mask_type;

	bridge = mem->bridge;
	if (!bridge)
		return -EINVAL;

	if (mem->page_count == 0)
		return 0;

	temp = bridge->current_size;

	switch (bridge->driver->size_type) {
	case U8_APER_SIZE:
		num_entries = A_SIZE_8(temp)->num_entries;
		break;
	case U16_APER_SIZE:
		num_entries = A_SIZE_16(temp)->num_entries;
		break;
	case U32_APER_SIZE:
		num_entries = A_SIZE_32(temp)->num_entries;
		break;
	case FIXED_APER_SIZE:
		num_entries = A_SIZE_FIX(temp)->num_entries;
		break;
	case LVL2_APER_SIZE:
		/* The generic routines can't deal with 2 level gatt's */
		return -EINVAL;
		break;
	default:
		num_entries = 0;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		break;
	}

	num_entries -= agp_memory_reserved/PAGE_SIZE;
	if (num_entries < 0) num_entries = 0;

	if (type != mem->type)
		return -EINVAL;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	mask_type = bridge->driver->agp_type_to_mask_type(bridge, type);
	if (mask_type != 0) {
		/* The generic routines know nothing of memory types */
		return -EINVAL;
	}

	if (((pg_start + mem->page_count) > num_entries) ||
	    ((pg_start + mem->page_count) < pg_start))
		return -EINVAL;

	j = pg_start;

	while (j < (pg_start + mem->page_count)) {
		if (!PGE_EMPTY(bridge, readl(bridge->gatt_table+j)))
			return -EBUSY;
		j++;
	}

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!mem->is_flushed) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		bridge->driver->cache_flush();
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		mem->is_flushed = true;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	for (i = 0, j = pg_start; i < mem->page_count; i++, j++) {
		writel(bridge->driver->mask_memory(bridge,
						   page_to_phys(mem->pages[i]),
						   mask_type),
		       bridge->gatt_table+j);
	}
	readl(bridge->gatt_table+j-1);	/* PCI Posting. */

	bridge->driver->tlb_flush(mem);
	return 0;
}
