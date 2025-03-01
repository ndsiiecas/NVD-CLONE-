static __be32 *CVE_2010_2521_PATCHED_read_buf(struct nfsd4_compoundargs *argp, u32 nbytes)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	/* We want more bytes than seem to be available.
	 * Maybe we need a new page, maybe we have just run out
	 */
	unsigned int avail = (char *)argp->end - (char *)argp->p;
	__be32 *p;
	if (avail + argp->pagelen < nbytes)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		return NULL;
	if (avail + PAGE_SIZE < nbytes) /* need more than a page !! */
		return NULL;
	/* ok, we can do it with the current plus the next page */
	if (nbytes <= sizeof(argp->tmp))
		p = argp->tmp;
	else {
		kfree(argp->tmpp);
		p = argp->tmpp = kmalloc(nbytes, GFP_KERNEL);
		if (!p)
			return NULL;
		
	}
	/*
	 * The following memcpy is safe because CVE_2010_2521_PATCHED_read_buf is always
	 * called with nbytes > avail, and the two cases above both
	 * guarantee p points to at least nbytes bytes.
	 */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	memcpy(p, argp->p, avail);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	/* step to next page */
	argp->p = page_address(argp->pagelist[0]);
	argp->pagelist++;
	if (argp->pagelen < PAGE_SIZE) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		argp->end = argp->p + (argp->pagelen>>2);
		argp->pagelen = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	} else {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		argp->end = argp->p + (PAGE_SIZE>>2);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		argp->pagelen -= PAGE_SIZE;
	}
	memcpy(((char*)p)+avail, argp->p, (nbytes - avail));
	argp->p += XDR_QUADLEN(nbytes - avail);
	return p;
}
