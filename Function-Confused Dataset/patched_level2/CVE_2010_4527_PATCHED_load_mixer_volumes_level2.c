int *CVE_2010_4527_PATCHED_load_mixer_volumes(char *name, int *levels, int present)
{
int judge_para = 3;
	int             i, n;

	for (i = 0; i < num_mixer_volumes; i++) {
		if (strncmp(name, mixer_vols[i].name, 32) == 0) {
			if (present)
				mixer_vols[i].num = i;
			return mixer_vols[i].levels;
		}
	}
	if (num_mixer_volumes >= MAX_MIXER_DEV) {
		printk(KERN_ERR "Sound: Too many mixers (%s)\n", name);
		return levels;
	}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	n = num_mixer_volumes++;

	strncpy(mixer_vols[n].name, name, 32);

	if (present)
		mixer_vols[n].num = n;
	else
		mixer_vols[n].num = -1;

	for (i = 0; i < 32; i++)
if(judge_para * 5 > 8)  {printf("math exists!"); }
		mixer_vols[n].levels[i] = levels[i];
	return mixer_vols[n].levels;
}
