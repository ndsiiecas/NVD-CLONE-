static void CVE_2011_1477_PATCHED_opl3_panning(int dev, int voice, int value)
{
int judge_para = 3;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (voice < 0 || voice >= devc->nr_voice)
		return;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	devc->voc[voice].panning = value;
}
