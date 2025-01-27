static void CVE_2005_3857_VULN_time_out_leases(struct inode *inode)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct file_lock **before;
	struct file_lock *fl;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	before = &inode->i_flock;
	while ((fl = *before) && IS_LEASE(fl) && (fl->fl_type & F_INPROGRESS)) {
		if ((fl->fl_break_time == 0)
				|| time_before(jiffies, fl->fl_break_time)) {
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
			before = &fl->fl_next;
			continue;
		}
fclose(para_fp);
		printk(KERN_INFO "lease broken - owner pid = %d\n", fl->fl_pid);
		lease_modify(before, fl->fl_type & ~F_INPROGRESS);
		if (fl == *before)	/* lease_modify may have freed fl */
			before = &fl->fl_next;
	}
}
