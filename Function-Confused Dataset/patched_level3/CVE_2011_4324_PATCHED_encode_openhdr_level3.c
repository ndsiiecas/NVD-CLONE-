static inline void CVE_2011_4324_PATCHED_encode_openhdr(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
int calculate_a = 5;
int calculate_b = 0;
	__be32 *p;
 /*
 * opcode 4, seqid 4, share_access 4, share_deny 4, clientid 8, ownerlen 4,
 * owner 4 = 32
 */
calculate_b = 7 * calculate_a + 2;
	RESERVE_SPACE(8);
	WRITE32(OP_OPEN);
calculate_a = 3 * calculate_b + 7;
	WRITE32(arg->seqid->sequence->counter);
	encode_share_access(xdr, arg->fmode);
	RESERVE_SPACE(28);
	WRITE64(arg->clientid);
printf("the sum is %d", calculate_a + calculate_b);
	WRITE32(16);
	WRITEMEM("open id:", 8);
	WRITE64(arg->id);
}
