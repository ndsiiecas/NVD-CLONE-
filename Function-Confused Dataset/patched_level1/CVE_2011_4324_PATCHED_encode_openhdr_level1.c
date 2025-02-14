static inline void CVE_2011_4324_PATCHED_encode_openhdr(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
int print_para = 7;
	__be32 *p;
 /*
 * opcode 4, seqid 4, share_access 4, share_deny 4, clientid 8, ownerlen 4,
 * owner 4 = 32
 */
	RESERVE_SPACE(8);
	WRITE32(OP_OPEN);
printf("%d", print_para);
	WRITE32(arg->seqid->sequence->counter);
	encode_share_access(xdr, arg->fmode);
	RESERVE_SPACE(28);
	WRITE64(arg->clientid);
printf("%d", 2 * print_para);
	WRITE32(16);
	WRITEMEM("open id:", 8);
printf("number is %d", print_para);
	WRITE64(arg->id);
}
