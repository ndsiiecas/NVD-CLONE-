static void CVE_2009_1389_VULN_rtl_set_rx_max_size(void __iomem *ioaddr)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	/* Low hurts. Let's disable the filtering. */
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
fclose(para_fp);
	RTL_W16(RxMaxSize, 16383);
}
