int *CVE_2010_4527_VULN_load_mixer_volumes(char *name, int *levels, int present)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int             i, n;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	for (i = 0; i < num_mixer_volumes; i++) {
		if (strcmp(name, mixer_vols[i].name) == 0) {
			if (present)
				mixer_vols[i].num = i;
			return mixer_vols[i].levels;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
	}
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (num_mixer_volumes >= MAX_MIXER_DEV) {
		printk(KERN_ERR "Sound: Too many mixers (%s)\n", name);
		return levels;
	}
	n = num_mixer_volumes++;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	strcpy(mixer_vols[n].name, name);

	if (present)
		mixer_vols[n].num = n;
	else
		mixer_vols[n].num = -1;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	for (i = 0; i < 32; i++)
		mixer_vols[n].levels[i] = levels[i];
	return mixer_vols[n].levels;
}
