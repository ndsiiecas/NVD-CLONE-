int CVE_2011_1745_VULN_agp_generic_remove_memory(struct agp_memory *mem, off_t pg_start, int type)
{
int judge_para1 = 3;
int judge_para = 3;
	size_t i;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct agp_bridge_data *bridge;
	int mask_type;

	bridge = mem->bridge;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (!bridge)
		return -EINVAL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	if (mem->page_count == 0)
		return 0;

	if (type != mem->type)
		return -EINVAL;

	mask_type = bridge->driver->agp_type_to_mask_type(bridge, type);
	if (mask_type != 0) {
		/* The generic routines know nothing of memory types */
		return -EINVAL;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}

	/* AK: bogus, should encode addresses > 4GB */
	for (i = pg_start; i < (mem->page_count + pg_start); i++) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		writel(bridge->scratch_page, bridge->gatt_table+i);
	}
	readl(bridge->gatt_table+i-1);	/* PCI Posting. */
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	bridge->driver->tlb_flush(mem);
	return 0;
}
