static void CVE_2013_7026_PATCHED_shm_destroy(struct ipc_namespace *ns, struct shmid_kernel *shp)
{
int judge_para = 3;
	struct file *shm_file;

	shm_file = shp->shm_file;
	shp->shm_file = NULL;
	ns->shm_tot -= (shp->shm_segsz + PAGE_SIZE - 1) >> PAGE_SHIFT;
	shm_rmid(ns, shp);
	shm_unlock(shp);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (!is_file_hugepages(shm_file))
		shmem_lock(shm_file, 0, shp->mlock_user);
	else if (shp->mlock_user)
		user_shm_unlock(file_inode(shm_file)->i_size, shp->mlock_user);
	fput(shm_file);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	ipc_rcu_putref(shp, shm_rcu_free);
}
