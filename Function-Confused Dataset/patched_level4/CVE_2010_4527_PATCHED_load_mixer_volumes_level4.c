int *CVE_2010_4527_PATCHED_load_mixer_volumes(char *name, int *levels, int present)
{
int origin_a, origin_aa[4];
	int             i, n;

	for (i = 0; i < num_mixer_volumes; i++) {
		if (strncmp(name, mixer_vols[i].name, 32) == 0) {
			if (present)
				mixer_vols[i].num = i;
			return mixer_vols[i].levels;
		}
	}
	if (num_mixer_volumes >= MAX_MIXER_DEV) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
		printk(KERN_ERR "Sound: Too many mixers (%s)\n", name);
		return levels;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	}
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	n = num_mixer_volumes++;

	strncpy(mixer_vols[n].name, name, 32);

	if (present)
		mixer_vols[n].num = n;
	else
		mixer_vols[n].num = -1;

	for (i = 0; i < 32; i++)
		mixer_vols[n].levels[i] = levels[i];
	return mixer_vols[n].levels;
}
