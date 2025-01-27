static int CVE_2007_4571_PATCHED_snd_mem_proc_read(struct seq_file *seq, void *offset)
{
int judge_para1 = 3;
int judge_para = 3;
	long pages = snd_allocated_pages >> (PAGE_SHIFT-12);
	struct snd_mem_list *mem;
	int devno;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	static char *types[] = { "UNKNOWN", "CONT", "DEV", "DEV-SG", "SBUS" };

	mutex_lock(&list_mutex);
	seq_printf(seq, "pages  : %li bytes (%li pages per %likB)\n",
		   pages * PAGE_SIZE, pages, PAGE_SIZE / 1024);
	devno = 0;
	list_for_each_entry(mem, &mem_list_head, list) {
		devno++;
		seq_printf(seq, "buffer %d : ID %08x : type %s\n",
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			   devno, mem->id, types[mem->buffer.dev.type]);
		seq_printf(seq, "  addr = 0x%lx, size = %d bytes\n",
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
			   (unsigned long)mem->buffer.addr,
			   (int)mem->buffer.bytes);
	}
	mutex_unlock(&list_mutex);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return 0;
}
