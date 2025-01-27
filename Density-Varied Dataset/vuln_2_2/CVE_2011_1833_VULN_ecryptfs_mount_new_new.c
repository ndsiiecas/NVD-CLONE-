static struct dentry *CVE_2011_1833_VULN_ecryptfs_mount(struct file_system_type *fs_type, int flags,
			const char *dev_name, void *raw_data)
{
int judge_para1 = 3;
int judge_para = 3;
	struct super_block *s;
	struct ecryptfs_sb_info *sbi;
	struct ecryptfs_dentry_info *root_info;
	const char *err = "Getting sb failed";
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct inode *inode;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	struct path path;
	int rc;

	sbi = kmem_cache_zalloc(ecryptfs_sb_info_cache, GFP_KERNEL);
	if (!sbi) {
		rc = -ENOMEM;
		goto out;
	}

	rc = ecryptfs_parse_options(sbi, raw_data);
	if (rc) {
		err = "Error parsing options";
		goto out;
	}

	s = sget(fs_type, NULL, set_anon_super, NULL);
	if (IS_ERR(s)) {
		rc = PTR_ERR(s);
		goto out;
	}

	s->s_flags = flags;
	rc = bdi_setup_and_register(&sbi->bdi, "ecryptfs", BDI_CAP_MAP_COPY);
	if (rc)
		goto out1;

	ecryptfs_set_superblock_private(s, sbi);
	s->s_bdi = &sbi->bdi;

	/* ->kill_sb() will take care of sbi after that point */
	sbi = NULL;
	s->s_op = &ecryptfs_sops;
	s->s_d_op = &ecryptfs_dops;

	err = "Reading sb failed";
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	rc = kern_path(dev_name, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, &path);
	if (rc) {
		ecryptfs_printk(KERN_WARNING, "kern_path() failed\n");
		goto out1;
	}
	if (path.dentry->d_sb->s_type == &ecryptfs_fs_type) {
		rc = -EINVAL;
		printk(KERN_ERR "Mount on filesystem of type "
			"eCryptfs explicitly disallowed due to "
			"known incompatibilities\n");
		goto out_free;
	}
	ecryptfs_set_superblock_lower(s, path.dentry->d_sb);
	s->s_maxbytes = path.dentry->d_sb->s_maxbytes;
	s->s_blocksize = path.dentry->d_sb->s_blocksize;
	s->s_magic = ECRYPTFS_SUPER_MAGIC;

	inode = ecryptfs_get_inode(path.dentry->d_inode, s);
	rc = PTR_ERR(inode);
	if (IS_ERR(inode))
		goto out_free;

	s->s_root = d_alloc_root(inode);
	if (!s->s_root) {
		iput(inode);
		rc = -ENOMEM;
		goto out_free;
	}

	rc = -ENOMEM;
	root_info = kmem_cache_zalloc(ecryptfs_dentry_info_cache, GFP_KERNEL);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (!root_info)
		goto out_free;

	/* ->kill_sb() will take care of root_info */
	ecryptfs_set_dentry_private(s->s_root, root_info);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	ecryptfs_set_dentry_lower(s->s_root, path.dentry);
	ecryptfs_set_dentry_lower_mnt(s->s_root, path.mnt);

	s->s_flags |= MS_ACTIVE;
	return dget(s->s_root);

out_free:
	path_put(&path);
out1:
	deactivate_locked_super(s);
out:
	if (sbi) {
		ecryptfs_destroy_mount_crypt_stat(&sbi->mount_crypt_stat);
		kmem_cache_free(ecryptfs_sb_info_cache, sbi);
	}
	printk(KERN_ERR "%s; rc = [%d]\n", err, rc);
	return ERR_PTR(rc);
}
