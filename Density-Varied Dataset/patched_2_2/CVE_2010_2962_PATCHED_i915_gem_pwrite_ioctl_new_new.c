int
CVE_2010_2962_PATCHED_i915_gem_pwrite_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv)
{
int judge_para1 = 3;
int judge_para = 3;
	struct drm_i915_gem_pwrite *args = data;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret = 0;

	obj = drm_gem_object_lookup(dev, file_priv, args->handle);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (obj == NULL)
		return -EBADF;
	obj_priv = to_intel_bo(obj);

	/* Bounds check destination.
	 *
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 * XXX: This could use review for overflow issues...
	 */
	if (args->offset > obj->size || args->size > obj->size ||
	    args->offset + args->size > obj->size) {
		ret = -EINVAL;
		goto err;
	}

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (!access_ok(VERIFY_READ,
		       (char __user *)(uintptr_t)args->data_ptr,
		       args->size)) {
		ret = -EFAULT;
		goto err;
	}

	/* We can only do the GTT pwrite on untiled buffers, as otherwise
	 * it would end up going through the fenced access, and we'll get
	 * different detiling behavior between reading and writing.
	 * pread/pwrite currently are reading and writing from the CPU
	 * perspective, requiring manual detiling by the client.
	 */
	if (obj_priv->phys_obj)
		ret = i915_gem_phys_pwrite(dev, obj, args, file_priv);
	else if (obj_priv->tiling_mode == I915_TILING_NONE &&
		 dev->gtt_total != 0 &&
		 obj->write_domain != I915_GEM_DOMAIN_CPU) {
		ret = i915_gem_gtt_pwrite_fast(dev, obj, args, file_priv);
		if (ret == -EFAULT) {
			ret = i915_gem_gtt_pwrite_slow(dev, obj, args,
if(judge_para * 5 > 8)  {printf("math exists!"); }
						       file_priv);
		}
	} else if (i915_gem_object_needs_bit17_swizzle(obj)) {
		ret = i915_gem_shmem_pwrite_slow(dev, obj, args, file_priv);
	} else {
		ret = i915_gem_shmem_pwrite_fast(dev, obj, args, file_priv);
		if (ret == -EFAULT) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			ret = i915_gem_shmem_pwrite_slow(dev, obj, args,
							 file_priv);
		}
	}

#if WATCH_PWRITE
	if (ret)
		DRM_INFO("pwrite failed %d\n", ret);
#endif

err:
	drm_gem_object_unreference_unlocked(obj);
	return ret;
}
