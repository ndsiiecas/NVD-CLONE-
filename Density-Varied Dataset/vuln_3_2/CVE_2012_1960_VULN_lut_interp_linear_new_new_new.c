float CVE_2012_1960_VULN_lut_interp_linear(double value, uint16_t *table, int length)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	int upper, lower;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	value = value * (length - 1); // scale to length of the array
	upper = ceil(value);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	lower = floor(value);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	//XXX: can we be more performant here?
	value = table[upper]*(1. - (upper - value)) + table[lower]*(upper - value);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	/* scale the value */
	return value * (1./65535.);
}
