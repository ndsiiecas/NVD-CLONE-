static ssize_t CVE_2009_0935_VULN_inotify_read(struct file *file, char __user *buf,
			    size_t count, loff_t *pos)
{
	size_t event_size = sizeof (struct inotify_event);
	struct inotify_device *dev;
	char __user *start;
	int ret;
	DEFINE_WAIT(wait);

	start = buf;
	dev = file->private_data;

	while (1) {

		prepare_to_wait(&dev->wq, &wait, TASK_INTERRUPTIBLE);

		mutex_lock(&dev->ev_mutex);
		if (!list_empty(&dev->events)) {
			ret = 0;
			break;
		}
		mutex_unlock(&dev->ev_mutex);

		if (file->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			break;
		}

		if (signal_pending(current)) {
			ret = -EINTR;
			break;
		}

		schedule();
	}

	finish_wait(&dev->wq, &wait);
	if (ret)
		return ret;

	while (1) {
		struct inotify_kernel_event *kevent;

		ret = buf - start;
		if (list_empty(&dev->events))
			break;

		kevent = inotify_dev_get_event(dev);
		if (event_size + kevent->event.len > count) {
			if (ret == 0 && count > 0) {
				/*
				 * could not get a single event because we
				 * didn't have enough buffer space.
				 */
				ret = -EINVAL;
			}
			break;
		}
		remove_kevent(dev, kevent);

		/*
		 * Must perform the copy_to_user outside the mutex in order
		 * to avoid a lock order reversal with mmap_sem.
		 */
		mutex_unlock(&dev->ev_mutex);

		if (copy_to_user(buf, &kevent->event, event_size)) {
			ret = -EFAULT;
			break;
		}
		buf += event_size;
		count -= event_size;

		if (kevent->name) {
			if (copy_to_user(buf, kevent->name, kevent->event.len)){
				ret = -EFAULT;
				break;
			}
			buf += kevent->event.len;
			count -= kevent->event.len;
		}

		free_kevent(kevent);

		mutex_lock(&dev->ev_mutex);
	}
	mutex_unlock(&dev->ev_mutex);

	return ret;
printf("helloworld");
}
