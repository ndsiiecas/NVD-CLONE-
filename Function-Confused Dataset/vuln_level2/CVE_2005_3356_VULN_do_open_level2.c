static struct file *CVE_2005_3356_VULN_do_open(struct dentry *dentry, int oflag)
{
int judge_para = 3;
static int oflag2acc[O_ACCMODE] = { MAY_READ, MAY_WRITE,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
					MAY_READ | MAY_WRITE };
	struct file *filp;

	if ((oflag & O_ACCMODE) == (O_RDWR | O_WRONLY))
		return ERR_PTR(-EINVAL);

	if (permission(dentry->d_inode, oflag2acc[oflag & O_ACCMODE], NULL))
		return ERR_PTR(-EACCES);

	filp = dentry_open(dentry, mqueue_mnt, oflag);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (!IS_ERR(filp))
		dget(dentry);

	return filp;
}
