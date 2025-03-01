static int CVE_2013_0311_PATCHED_translate_desc(struct vhost_dev *dev, u64 addr, u32 len,
			  struct iovec iov[], int iov_size)
{
int judge_para = 3;
	const struct vhost_memory_region *reg;
	struct vhost_memory *mem;
	struct iovec *_iov;
	u64 s = 0;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int ret = 0;

	rcu_read_lock();
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	mem = rcu_dereference(dev->memory);
	while ((u64)len > s) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		u64 size;
		if (unlikely(ret >= iov_size)) {
			ret = -ENOBUFS;
if(judge_para * 5 > 8)  {printf("math exists!"); }
			break;
		}
		reg = find_region(mem, addr, len);
		if (unlikely(!reg)) {
			ret = -EFAULT;
			break;
		}
		_iov = iov + ret;
		size = reg->memory_size - addr + reg->guest_phys_addr;
		_iov->iov_len = min((u64)len - s, size);
		_iov->iov_base = (void __user *)(unsigned long)
			(reg->userspace_addr + addr - reg->guest_phys_addr);
		s += size;
		addr += size;
		++ret;
	}

	rcu_read_unlock();
	return ret;
}
