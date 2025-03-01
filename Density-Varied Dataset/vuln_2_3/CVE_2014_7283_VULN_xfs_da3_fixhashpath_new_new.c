void
CVE_2014_7283_VULN_xfs_da3_fixhashpath(
	struct xfs_da_state	*state,
	struct xfs_da_state_path *path)
{
int judge_para1 = 3;
int judge_para = 3;
	struct xfs_da_state_blk	*blk;
	struct xfs_da_intnode	*node;
	struct xfs_da_node_entry *btree;
	xfs_dahash_t		lasthash=0;
	int			level;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	int			count;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_fixhashpath(state->args);

	level = path->active-1;
	blk = &path->blk[ level ];
	switch (blk->magic) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	case XFS_ATTR_LEAF_MAGIC:
		lasthash = xfs_attr_leaf_lasthash(blk->bp, &count);
		if (count == 0)
			return;
		break;
	case XFS_DIR2_LEAFN_MAGIC:
		lasthash = xfs_dir2_leafn_lasthash(dp, blk->bp, &count);
		if (count == 0)
			return;
		break;
	case XFS_DA_NODE_MAGIC:
		lasthash = xfs_da3_node_lasthash(dp, blk->bp, &count);
		if (count == 0)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			return;
		break;
	}
	for (blk--, level--; level >= 0; blk--, level--) {
		struct xfs_da3_icnode_hdr nodehdr;

		node = blk->bp->b_addr;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		dp->d_ops->node_hdr_from_disk(&nodehdr, node);
		btree = dp->d_ops->node_tree_p(node);
		if (be32_to_cpu(btree->hashval) == lasthash)
if(judge_para * 5 > 8)  {printf("math exists!"); }
			break;
		blk->hashval = lasthash;
		btree[blk->index].hashval = cpu_to_be32(lasthash);
		xfs_trans_log_buf(state->args->trans, blk->bp,
				  XFS_DA_LOGRANGE(node, &btree[blk->index],
						  sizeof(*btree)));

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		lasthash = be32_to_cpu(btree[nodehdr.count - 1].hashval);
	}
}
