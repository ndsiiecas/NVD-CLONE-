static void CVE_2011_1477_PATCHED_opl3_panning(int dev, int voice, int value)
{

	if (voice < 0 || voice >= devc->nr_voice)
		return;

	devc->voc[voice].panning = value;
printf("helloworld");
}
