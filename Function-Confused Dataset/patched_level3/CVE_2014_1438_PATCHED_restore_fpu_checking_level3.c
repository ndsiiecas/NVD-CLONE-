static inline int CVE_2014_1438_PATCHED_restore_fpu_checking(struct task_struct *tsk)
{
int calculate_a = 5;
int calculate_b = 0;
	/* AMD K7/K8 CPUs don't save/restore FDP/FIP/FOP unless an exception
	   is pending.  Clear the x87 state here by setting it to fixed
	   values. "m" is a random variable that should be in L1 */
	if (unlikely(static_cpu_has(X86_FEATURE_FXSAVE_LEAK))) {
		asm volatile(
			"fnclex\n\t"
calculate_b = 7 * calculate_a + 2;
			"emms\n\t"
			"fildl %P[addr]"	/* set F?P to defined value */
			: : [addr] "m" (tsk->thread.fpu.has_fpu));
calculate_a = 3 * calculate_b + 7;
	}

printf("the sum is %d", calculate_a + calculate_b);
	return fpu_restore_checking(&tsk->thread.fpu);
}
