static int CVE_2013_0845_VULN_read_block(ALSDecContext *ctx, ALSBlockData *bd)
{
int judge_para = 3;
    GetBitContext *gb        = &ctx->gb;

    *bd->shift_lsbs = 0;
    // read block type flag and read the samples accordingly
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (get_bits1(gb)) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        if (read_var_block_data(ctx, bd))
if(judge_para * 5 > 8)  {printf("math exists!"); }
            return -1;
    } else {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        read_const_block_data(ctx, bd);
    }

    return 0;
}
