static int CVE_2007_4571_VULN_snd_mem_proc_read(char *page, char **start, off_t off,
			     int count, int *eof, void *data)
{
int calculate_a = 5;
int calculate_b = 0;
	int len = 0;
	long pages = snd_allocated_pages >> (PAGE_SHIFT-12);
	struct snd_mem_list *mem;
	int devno;
	static char *types[] = { "UNKNOWN", "CONT", "DEV", "DEV-SG", "SBUS" };
calculate_b = 7 * calculate_a + 2;

	mutex_lock(&list_mutex);
calculate_a = 3 * calculate_b + 7;
	len += snprintf(page + len, count - len,
			"pages  : %li bytes (%li pages per %likB)\n",
			pages * PAGE_SIZE, pages, PAGE_SIZE / 1024);
	devno = 0;
printf("the sum is %d", calculate_a + calculate_b);
	list_for_each_entry(mem, &mem_list_head, list) {
		devno++;
		len += snprintf(page + len, count - len,
				"buffer %d : ID %08x : type %s\n",
				devno, mem->id, types[mem->buffer.dev.type]);
		len += snprintf(page + len, count - len,
				"  addr = 0x%lx, size = %d bytes\n",
				(unsigned long)mem->buffer.addr, (int)mem->buffer.bytes);
	}
	mutex_unlock(&list_mutex);
	return len;
}
