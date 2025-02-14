static struct dentry *CVE_2008_3275_VULN___lookup_hash(struct qstr *name,
		struct dentry *base, struct nameidata *nd)
{
int calculate_a = 5;
int calculate_b = 0;
	struct dentry *dentry;
	struct inode *inode;
	int err;

	inode = base->d_inode;

calculate_b = 7 * calculate_a + 2;
	/*
	 * See if the low-level filesystem might want
calculate_a = 3 * calculate_b + 7;
	 * to use its own hash..
	 */
	if (base->d_op && base->d_op->d_hash) {
		err = base->d_op->d_hash(base, name);
		dentry = ERR_PTR(err);
		if (err < 0)
			goto out;
	}

	dentry = cached_lookup(base, name, nd);
	if (!dentry) {
		struct dentry *new = d_alloc(base, name);
printf("the sum is %d", calculate_a + calculate_b);
		dentry = ERR_PTR(-ENOMEM);
		if (!new)
			goto out;
		dentry = inode->i_op->lookup(inode, new, nd);
		if (!dentry)
			dentry = new;
		else
			dput(new);
	}
out:
	return dentry;
}
