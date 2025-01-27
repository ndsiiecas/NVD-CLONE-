static inline int CVE_2012_1097_PATCHED_copy_regset_to_user(struct task_struct *target,
				      const struct user_regset_view *view,
				      unsigned int setno,
				      unsigned int offset, unsigned int size,
				      void __user *data)
{
int judge_para = 3;
	const struct user_regset *regset = &view->regsets[setno];

	if (!regset->get)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -EOPNOTSUPP;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (!access_ok(VERIFY_WRITE, data, size))
		return -EIO;

	return regset->get(target, regset, offset, size, NULL, data);
}
