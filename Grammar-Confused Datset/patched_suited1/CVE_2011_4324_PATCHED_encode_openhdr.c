static inline void CVE_2011_4324_PATCHED_encode_openhdr(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
	__be32 *p;
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
 /*
 * opcode 4, seqid 4, share_access 4, share_deny 4, clientid 8, ownerlen 4,
 * owner 4 = 32
 */
	RESERVE_SPACE(8);
	WRITE32(OP_OPEN);
	WRITE32(arg->seqid->sequence->counter);
	encode_share_access(xdr, arg->fmode);
	RESERVE_SPACE(28);
	WRITE64(arg->clientid);
	WRITE32(16);
	WRITEMEM("open id:", 8);
	WRITE64(arg->id);
}
