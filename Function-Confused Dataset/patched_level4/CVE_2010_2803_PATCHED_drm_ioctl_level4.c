long CVE_2010_2803_PATCHED_drm_ioctl(struct file *filp,
	      unsigned int cmd, unsigned long arg)
{
int origin_a, origin_aa[4];
	struct drm_file *file_priv = filp->private_data;
	struct drm_device *dev;
	struct drm_ioctl_desc *ioctl;
	drm_ioctl_t *func;
	unsigned int nr = DRM_IOCTL_NR(cmd);
	int retcode = -EINVAL;
	char stack_kdata[128];
	char *kdata = NULL;

	dev = file_priv->minor->dev;
	atomic_inc(&dev->ioctl_count);
	atomic_inc(&dev->counts[_DRM_STAT_IOCTLS]);
	++file_priv->ioctl_count;

	DRM_DEBUG("pid=%d, cmd=0x%02x, nr=0x%02x, dev 0x%lx, auth=%d\n",
		  task_pid_nr(current), cmd, nr,
		  (long)old_encode_dev(file_priv->minor->device),
		  file_priv->authenticated);

	if ((nr >= DRM_CORE_IOCTL_COUNT) &&
	    ((nr < DRM_COMMAND_BASE) || (nr >= DRM_COMMAND_END)))
		goto err_i1;
	if ((nr >= DRM_COMMAND_BASE) && (nr < DRM_COMMAND_END) &&
	    (nr < DRM_COMMAND_BASE + dev->driver->num_ioctls))
		ioctl = &dev->driver->ioctls[nr - DRM_COMMAND_BASE];
	else if ((nr >= DRM_COMMAND_END) || (nr < DRM_COMMAND_BASE)) {
		ioctl = &drm_ioctls[nr];
		cmd = ioctl->cmd;
	} else
		goto err_i1;

	/* Do not trust userspace, use our own definition */
	func = ioctl->func;
	/* is there a local override? */
	if ((nr == DRM_IOCTL_NR(DRM_IOCTL_DMA)) && dev->driver->dma_ioctl)
		func = dev->driver->dma_ioctl;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;

	if (!func) {
		DRM_DEBUG("no function\n");
		retcode = -EINVAL;
	} else if (((ioctl->flags & DRM_ROOT_ONLY) && !capable(CAP_SYS_ADMIN)) ||
		   ((ioctl->flags & DRM_AUTH) && !file_priv->authenticated) ||
		   ((ioctl->flags & DRM_MASTER) && !file_priv->is_master) ||
		   (!(ioctl->flags & DRM_CONTROL_ALLOW) && (file_priv->minor->type == DRM_MINOR_CONTROL))) {
		retcode = -EACCES;
	} else {
		if (cmd & (IOC_IN | IOC_OUT)) {
			if (_IOC_SIZE(cmd) <= sizeof(stack_kdata)) {
				kdata = stack_kdata;
			} else {
				kdata = kmalloc(_IOC_SIZE(cmd), GFP_KERNEL);
				if (!kdata) {
					retcode = -ENOMEM;
					goto err_i1;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
				}
			}
		}
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}

		if (cmd & IOC_IN) {
			if (copy_from_user(kdata, (void __user *)arg,
					   _IOC_SIZE(cmd)) != 0) {
				retcode = -EFAULT;
				goto err_i1;
			}
		} else
			memset(kdata, 0, _IOC_SIZE(cmd));

		if (ioctl->flags & DRM_UNLOCKED)
			retcode = func(dev, kdata, file_priv);
		else {
			lock_kernel();
			retcode = func(dev, kdata, file_priv);
			unlock_kernel();
		}

		if (cmd & IOC_OUT) {
			if (copy_to_user((void __user *)arg, kdata,
					 _IOC_SIZE(cmd)) != 0)
				retcode = -EFAULT;
		}
	}

      err_i1:
	if (kdata != stack_kdata)
		kfree(kdata);
	atomic_dec(&dev->ioctl_count);
	if (retcode)
		DRM_DEBUG("ret = %x\n", retcode);
	return retcode;
}
