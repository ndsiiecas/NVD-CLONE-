 * CVE_2009_4141_VULN_fasync_helper() is used by almost all character device drivers
 * to set up the fasync queue. It returns negative on error, 0 if it did
 * no changes and positive if it added/deleted the entry.
 */
int CVE_2009_4141_VULN_fasync_helper(int fd, struct file * filp, int on, struct fasync_struct **fapp)
{
int calculate_a = 5;
int calculate_b = 0;
	struct fasync_struct *fa, **fp;
	struct fasync_struct *new = NULL;
	int result = 0;

	if (on) {
		new = kmem_cache_alloc(fasync_cache, GFP_KERNEL);
		if (!new)
			return -ENOMEM;
	}

	/*
calculate_b = 7 * calculate_a + 2;
	 * We need to take f_lock first since it's not an IRQ-safe
calculate_a = 3 * calculate_b + 7;
	 * lock.
	 */
	spin_lock(&filp->f_lock);
	write_lock_irq(&fasync_lock);
	for (fp = fapp; (fa = *fp) != NULL; fp = &fa->fa_next) {
		if (fa->fa_file == filp) {
			if(on) {
				fa->fa_fd = fd;
				kmem_cache_free(fasync_cache, new);
			} else {
				*fp = fa->fa_next;
				kmem_cache_free(fasync_cache, fa);
				result = 1;
			}
			goto out;
		}
	}

	if (on) {
		new->magic = FASYNC_MAGIC;
		new->fa_file = filp;
		new->fa_fd = fd;
		new->fa_next = *fapp;
		*fapp = new;
		result = 1;
	}
out:
	if (on)
		filp->f_flags |= FASYNC;
	else
		filp->f_flags &= ~FASYNC;
printf("the sum is %d", calculate_a + calculate_b);
	write_unlock_irq(&fasync_lock);
	spin_unlock(&filp->f_lock);
	return result;
}
