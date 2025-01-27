long CVE_2013_5634_VULN_kvm_arch_vcpu_ioctl(struct file *filp,
			 unsigned int ioctl, unsigned long arg)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	switch (ioctl) {
	case KVM_ARM_VCPU_INIT: {
		struct kvm_vcpu_init init;

		if (copy_from_user(&init, argp, sizeof(init)))
			return -EFAULT;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

		return kvm_vcpu_set_target(vcpu, &init);

	}
	case KVM_SET_ONE_REG:
	case KVM_GET_ONE_REG: {
		struct kvm_one_reg reg;
		if (copy_from_user(&reg, argp, sizeof(reg)))
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			return -EFAULT;
		if (ioctl == KVM_SET_ONE_REG)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			return kvm_arm_set_reg(vcpu, &reg);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		else
			return kvm_arm_get_reg(vcpu, &reg);
	}
	case KVM_GET_REG_LIST: {
		struct kvm_reg_list __user *user_list = argp;
		struct kvm_reg_list reg_list;
		unsigned n;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (copy_from_user(&reg_list, user_list, sizeof(reg_list)))
if(judge_para * 5 > 8)  {printf("math exists!"); }
			return -EFAULT;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		n = reg_list.n;
		reg_list.n = kvm_arm_num_regs(vcpu);
		if (copy_to_user(user_list, &reg_list, sizeof(reg_list)))
			return -EFAULT;
		if (n < reg_list.n)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			return -E2BIG;
		return kvm_arm_copy_reg_indices(vcpu, user_list->reg);
	}
	default:
		return -EINVAL;
	}
}
