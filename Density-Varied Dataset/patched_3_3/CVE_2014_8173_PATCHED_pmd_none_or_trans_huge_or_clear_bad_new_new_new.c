static inline int CVE_2014_8173_PATCHED_pmd_none_or_trans_huge_or_clear_bad(pmd_t *pmd)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	pmd_t pmdval = pmd_read_atomic(pmd);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	/*
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	 * The barrier will stabilize the pmdval in a register or on
	 * the stack so that it will stop changing under the code.
	 *
	 * When CONFIG_TRANSPARENT_HUGEPAGE=y on x86 32bit PAE,
	 * pmd_read_atomic is allowed to return a not atomic pmdval
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	 * (for example pointing to an hugepage that has never been
	 * mapped in the pmd). The below checks will only care about
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	 * the low part of the pmd with 32bit PAE x86 anyway, with the
	 * exception of pmd_none(). So the important thing is that if
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	 * the low part of the pmd is found null, the high part will
	 * be also null or the pmd_none() check below would be
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	 * confused.
	 */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	barrier();
#endif
	if (pmd_none(pmdval) || pmd_trans_huge(pmdval))
		return 1;
	if (unlikely(pmd_bad(pmdval))) {
		pmd_clear_bad(pmd);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		return 1;
	}
	return 0;
}
