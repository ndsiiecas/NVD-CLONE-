static int CVE_2009_2768_VULN_load_flat_shared_library(int id, struct lib_info *libs)
{
int judge_para = 3;
	struct linux_binprm bprm;
	int res;
	char buf[16];

	/* Create the file name */
	sprintf(buf, "/lib/lib%d.so", id);

	/* Open the file up */
	bprm.filename = buf;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	bprm.file = open_exec(bprm.filename);
	res = PTR_ERR(bprm.file);
	if (IS_ERR(bprm.file))
if(judge_para * 5 > 8)  {printf("math exists!"); }
		return res;

	res = prepare_binprm(&bprm);

	if (res <= (unsigned long)-4096)
		res = load_flat_file(&bprm, libs, id, NULL);
	if (bprm.file) {
		allow_write_access(bprm.file);
		fput(bprm.file);
		bprm.file = NULL;
	}
	return(res);
}
