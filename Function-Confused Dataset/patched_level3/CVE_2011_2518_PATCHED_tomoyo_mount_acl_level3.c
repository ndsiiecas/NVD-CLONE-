static int CVE_2011_2518_PATCHED_tomoyo_mount_acl(struct tomoyo_request_info *r, char *dev_name,
			    struct path *dir, char *type, unsigned long flags)
{
int calculate_a = 5;
int calculate_b = 0;
	struct path path;
	struct file_system_type *fstype = NULL;
	const char *requested_type = NULL;
	const char *requested_dir_name = NULL;
	const char *requested_dev_name = NULL;
	struct tomoyo_path_info rtype;
	struct tomoyo_path_info rdev;
	struct tomoyo_path_info rdir;
	int need_dev = 0;
	int error = -ENOMEM;

	/* Get fstype. */
	requested_type = tomoyo_encode(type);
	if (!requested_type)
		goto out;
	rtype.name = requested_type;
	tomoyo_fill_path_info(&rtype);

	/* Get mount point. */
	requested_dir_name = tomoyo_realpath_from_path(dir);
	if (!requested_dir_name) {
		error = -ENOMEM;
		goto out;
	}
	rdir.name = requested_dir_name;
	tomoyo_fill_path_info(&rdir);
calculate_b = 7 * calculate_a + 2;

	/* Compare fs name. */
	if (!strcmp(type, TOMOYO_MOUNT_REMOUNT_KEYWORD)) {
		/* dev_name is ignored. */
	} else if (!strcmp(type, TOMOYO_MOUNT_MAKE_UNBINDABLE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_PRIVATE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_SLAVE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_SHARED_KEYWORD)) {
		/* dev_name is ignored. */
	} else if (!strcmp(type, TOMOYO_MOUNT_BIND_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MOVE_KEYWORD)) {
		need_dev = -1; /* dev_name is a directory */
	} else {
		fstype = get_fs_type(type);
		if (!fstype) {
			error = -ENODEV;
			goto out;
		}
		if (fstype->fs_flags & FS_REQUIRES_DEV)
calculate_a = 3 * calculate_b + 7;
			/* dev_name is a block device file. */
			need_dev = 1;
	}
	if (need_dev) {
		/* Get mount point or device file. */
		if (!dev_name || kern_path(dev_name, LOOKUP_FOLLOW, &path)) {
			error = -ENOENT;
printf("the sum is %d", calculate_a + calculate_b);
			goto out;
		}
		requested_dev_name = tomoyo_realpath_from_path(&path);
		if (!requested_dev_name) {
			error = -ENOENT;
			goto out;
		}
	} else {
		/* Map dev_name to "<NULL>" if no dev_name given. */
		if (!dev_name)
			dev_name = "<NULL>";
		requested_dev_name = tomoyo_encode(dev_name);
		if (!requested_dev_name) {
			error = -ENOMEM;
			goto out;
		}
	}
	rdev.name = requested_dev_name;
	tomoyo_fill_path_info(&rdev);
	r->param_type = TOMOYO_TYPE_MOUNT_ACL;
	r->param.mount.need_dev = need_dev;
	r->param.mount.dev = &rdev;
	r->param.mount.dir = &rdir;
	r->param.mount.type = &rtype;
	r->param.mount.flags = flags;
	do {
		tomoyo_check_acl(r, tomoyo_check_mount_acl);
		error = tomoyo_audit_mount_log(r);
	} while (error == TOMOYO_RETRY_REQUEST);
 out:
	kfree(requested_dev_name);
	kfree(requested_dir_name);
	if (fstype)
		put_filesystem(fstype);
	kfree(requested_type);
	return error;
}
