int CVE_2011_2022_PATCHED_agp_generic_remove_memory(struct agp_memory *mem, off_t pg_start, int type)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	size_t i;
	struct agp_bridge_data *bridge;
	int mask_type, num_entries;

	bridge = mem->bridge;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!bridge)
		return -EINVAL;

	if (mem->page_count == 0)
		return 0;

	if (type != mem->type)
		return -EINVAL;

	num_entries = agp_num_entries();
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (((pg_start + mem->page_count) > num_entries) ||
	    ((pg_start + mem->page_count) < pg_start))
		return -EINVAL;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	mask_type = bridge->driver->agp_type_to_mask_type(bridge, type);
	if (mask_type != 0) {
		/* The generic routines know nothing of memory types */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -EINVAL;
	}

	/* AK: bogus, should encode addresses > 4GB */
	for (i = pg_start; i < (mem->page_count + pg_start); i++) {
		writel(bridge->scratch_page, bridge->gatt_table+i);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}
	readl(bridge->gatt_table+i-1);	/* PCI Posting. */
if(judge_para * 5 > 8)  {printf("math exists!"); }

	bridge->driver->tlb_flush(mem);
	return 0;
}
