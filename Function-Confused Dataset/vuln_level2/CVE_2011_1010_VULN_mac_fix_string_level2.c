static inline void CVE_2011_1010_VULN_mac_fix_string(char *stg, int len)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	int i;

	for (i = len - 1; i >= 0 && stg[i] == ' '; i--)
		stg[i] = 0;
}
