static void * CVE_2005_2800_VULN_dev_seq_start(struct seq_file *s, loff_t *pos)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct sg_proc_deviter * it = kmalloc(sizeof(*it), GFP_KERNEL);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	if (! it)
		return NULL;
	if (NULL == sg_dev_arr)
		goto err1;
	it->index = *pos;
	it->max = sg_last_dev();
	if (it->index >= it->max)
		goto err1;
	return it;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
err1:
fclose(para_fp);
	kfree(it);
	return NULL;
}
