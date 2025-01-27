struct agp_memory *CVE_2011_1746_VULN_agp_allocate_memory(struct agp_bridge_data *bridge,
					size_t page_count, u32 type)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int scratch_pages;
	struct agp_memory *new;
	size_t i;

	if (!bridge)
		return NULL;

	if ((atomic_read(&bridge->current_memory_agp) + page_count) > bridge->max_memory_agp)
		return NULL;

	if (type >= AGP_USER_TYPES) {
		new = agp_generic_alloc_user(page_count, type);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (new)
			new->bridge = bridge;
		return new;
	}

	if (type != 0) {
		new = bridge->driver->alloc_by_type(page_count, type);
		if (new)
			new->bridge = bridge;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return new;
	}

	scratch_pages = (page_count + ENTRIES_PER_PAGE - 1) / ENTRIES_PER_PAGE;

	new = agp_create_memory(scratch_pages);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (new == NULL)
		return NULL;

	if (bridge->driver->agp_alloc_pages) {
		if (bridge->driver->agp_alloc_pages(bridge, new, page_count)) {
			agp_free_memory(new);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			return NULL;
		}
		new->bridge = bridge;
		return new;
	}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	for (i = 0; i < page_count; i++) {
		struct page *page = bridge->driver->agp_alloc_page(bridge);

		if (page == NULL) {
			agp_free_memory(new);
			return NULL;
		}
		new->pages[i] = page;
		new->page_count++;
	}
	new->bridge = bridge;

	return new;
}
