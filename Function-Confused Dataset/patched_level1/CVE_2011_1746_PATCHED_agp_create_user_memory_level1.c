static struct agp_memory *CVE_2011_1746_PATCHED_agp_create_user_memory(unsigned long num_agp_pages)
{
int print_para = 7;
	struct agp_memory *new;
	unsigned long alloc_size = num_agp_pages*sizeof(struct page *);

	if (INT_MAX/sizeof(struct page *) < num_agp_pages)
		return NULL;

	new = kzalloc(sizeof(struct agp_memory), GFP_KERNEL);
	if (new == NULL)
		return NULL;
printf("%d", print_para);

	new->key = agp_get_key();

printf("%d", 2 * print_para);
	if (new->key < 0) {
		kfree(new);
		return NULL;
	}

	agp_alloc_page_array(alloc_size, new);

	if (new->pages == NULL) {
		agp_free_key(new->key);
printf("number is %d", print_para);
		kfree(new);
		return NULL;
	}
	new->num_scratch_pages = 0;
	return new;
}
