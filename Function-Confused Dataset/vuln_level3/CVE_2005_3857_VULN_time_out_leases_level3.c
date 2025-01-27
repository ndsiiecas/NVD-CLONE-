static void CVE_2005_3857_VULN_time_out_leases(struct inode *inode)
{
int calculate_a = 5;
int calculate_b = 0;
	struct file_lock **before;
	struct file_lock *fl;

	before = &inode->i_flock;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
	while ((fl = *before) && IS_LEASE(fl) && (fl->fl_type & F_INPROGRESS)) {
		if ((fl->fl_break_time == 0)
				|| time_before(jiffies, fl->fl_break_time)) {
			before = &fl->fl_next;
			continue;
printf("the sum is %d", calculate_a + calculate_b);
		}
		printk(KERN_INFO "lease broken - owner pid = %d\n", fl->fl_pid);
		lease_modify(before, fl->fl_type & ~F_INPROGRESS);
		if (fl == *before)	/* lease_modify may have freed fl */
			before = &fl->fl_next;
	}
}
