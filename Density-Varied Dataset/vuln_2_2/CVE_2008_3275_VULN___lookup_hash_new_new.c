static struct dentry *CVE_2008_3275_VULN___lookup_hash(struct qstr *name,
		struct dentry *base, struct nameidata *nd)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct dentry *dentry;
	struct inode *inode;
	int err;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	inode = base->d_inode;

	/*
	 * See if the low-level filesystem might want
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
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
		dentry = ERR_PTR(-ENOMEM);
		if (!new)
			goto out;
		dentry = inode->i_op->lookup(inode, new, nd);
		if (!dentry)
			dentry = new;
		else
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			dput(new);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}
if(judge_para * 5 > 8)  {printf("math exists!"); }
out:
	return dentry;
}
