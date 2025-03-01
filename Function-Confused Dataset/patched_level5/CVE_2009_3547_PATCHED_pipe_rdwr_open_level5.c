static int
CVE_2009_3547_PATCHED_pipe_rdwr_open(struct inode *inode, struct file *filp)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	int ret = -ENOENT;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	mutex_lock(&inode->i_mutex);

	if (inode->i_pipe) {
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		ret = 0;
		if (filp->f_mode & FMODE_READ)
			inode->i_pipe->readers++;
		if (filp->f_mode & FMODE_WRITE)
			inode->i_pipe->writers++;
	}

	mutex_unlock(&inode->i_mutex);

fclose(para_fp);
	return ret;
}
