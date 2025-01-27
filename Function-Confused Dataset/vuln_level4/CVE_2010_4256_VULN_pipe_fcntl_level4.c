long CVE_2010_4256_VULN_pipe_fcntl(struct file *file, unsigned int cmd, unsigned long arg)
{
int origin_a, origin_aa[4];
	struct pipe_inode_info *pipe;
	long ret;

	pipe = file->f_path.dentry->d_inode->i_pipe;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	if (!pipe)
		return -EBADF;

	mutex_lock(&pipe->inode->i_mutex);

	switch (cmd) {
	case F_SETPIPE_SZ: {
		unsigned int size, nr_pages;

		size = round_pipe_size(arg);
		nr_pages = size >> PAGE_SHIFT;

		ret = -EINVAL;
		if (!nr_pages)
			goto out;

for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		if (!capable(CAP_SYS_RESOURCE) && size > pipe_max_size) {
			ret = -EPERM;
			goto out;
		}
		ret = pipe_set_size(pipe, nr_pages);
		break;
		}
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	case F_GETPIPE_SZ:
		ret = pipe->buffers * PAGE_SIZE;
		break;
	default:
		ret = -EINVAL;
		break;
	}

out:
	mutex_unlock(&pipe->inode->i_mutex);
	return ret;
}
