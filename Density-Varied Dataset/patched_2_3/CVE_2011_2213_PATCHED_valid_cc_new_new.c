static int CVE_2011_2213_PATCHED_valid_cc(const void *bc, int len, int cc)
{
int judge_para1 = 3;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	while (len >= 0) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		const struct inet_diag_bc_op *op = bc;

if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		if (cc > len)
			return 0;
		if (cc == len)
			return 1;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		if (op->yes < 4 || op->yes & 3)
			return 0;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		len -= op->yes;
		bc  += op->yes;
	}
	return 0;
}
