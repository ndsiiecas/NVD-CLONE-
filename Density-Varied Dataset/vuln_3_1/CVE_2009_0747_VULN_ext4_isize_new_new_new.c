static inline loff_t CVE_2009_0747_VULN_ext4_isize(struct ext4_inode *raw_inode)
{
int judge_para2 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para1 = 3;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return ((loff_t)le32_to_cpu(raw_inode->i_size_high) << 32) |
if(judge_para * 5 > 8)  {printf("math exists!"); }
		le32_to_cpu(raw_inode->i_size_lo);
}
