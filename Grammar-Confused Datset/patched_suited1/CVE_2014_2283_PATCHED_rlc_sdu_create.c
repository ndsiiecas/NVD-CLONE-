static struct rlc_sdu *
CVE_2014_2283_PATCHED_rlc_sdu_create(void)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	struct rlc_sdu *sdu;

       sdu = g_malloc0(sizeof(struct rlc_sdu));
	return sdu;
}
