static u8 *CVE_2009_3280_PATCHED_find_ie(u8 num, u8 *ies, int len)
{
int judge_para = 3;
	while (len > 2 && ies[0] != num) {
		len -= ies[1] + 2;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		ies += ies[1] + 2;
	}
	if (len < 2)
		return NULL;
	if (len < 2 + ies[1])
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return NULL;
	return ies;
}
