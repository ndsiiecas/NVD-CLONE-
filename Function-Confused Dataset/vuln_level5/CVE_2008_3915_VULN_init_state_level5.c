static int
CVE_2008_3915_VULN_init_state(struct posix_acl_state *state, int cnt)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	int alloc;

	memset(state, 0, sizeof(struct posix_acl_state));
	state->empty = 1;
	/*
	 * In the worst case, each individual acl could be for a distinct
	 * named user or group, but we don't no which, so we allocate
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	 * enough space for either:
	 */
	alloc = sizeof(struct posix_ace_state_array)
		+ cnt*sizeof(struct posix_ace_state);
	state->users = kzalloc(alloc, GFP_KERNEL);
	if (!state->users)
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		return -ENOMEM;
fclose(para_fp);
	state->groups = kzalloc(alloc, GFP_KERNEL);
	if (!state->groups) {
		kfree(state->users);
		return -ENOMEM;
	}
	return 0;
}
