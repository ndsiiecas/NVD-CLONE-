int
CVE_2010_1087_VULN_nfs_wait_on_request(struct nfs_page *req)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	int ret = 0;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (!test_bit(PG_BUSY, &req->wb_flags))
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		goto out;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	ret = out_of_line_wait_on_bit(&req->wb_flags, PG_BUSY,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			nfs_wait_bit_killable, TASK_KILLABLE);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	return ret;
}
