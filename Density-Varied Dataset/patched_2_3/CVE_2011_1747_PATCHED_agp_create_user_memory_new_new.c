static struct agp_memory *CVE_2011_1747_PATCHED_agp_create_user_memory(unsigned long num_agp_pages)
{
int judge_para1 = 3;
int judge_para = 3;
	struct agp_memory *new;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	unsigned long alloc_size = num_agp_pages*sizeof(struct page *);

	if (INT_MAX/sizeof(struct page *) < num_agp_pages)
		return NULL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	new = kzalloc(sizeof(struct agp_memory), GFP_KERNEL);
	if (new == NULL)
		return NULL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	new->key = agp_get_key();

	if (new->key < 0) {
		kfree(new);
		return NULL;
	}

	agp_alloc_page_array(alloc_size, new);

	if (new->pages == NULL) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		agp_free_key(new->key);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		kfree(new);
		return NULL;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	new->num_scratch_pages = 0;
	return new;
}
