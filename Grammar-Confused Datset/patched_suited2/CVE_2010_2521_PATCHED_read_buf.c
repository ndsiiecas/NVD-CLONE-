static __be32 *CVE_2010_2521_PATCHED_read_buf(struct nfsd4_compoundargs *argp, u32 nbytes)
{
	/* We want more bytes than seem to be available.
	 * Maybe we need a new page, maybe we have just run out
	 */
	unsigned int avail = (char *)argp->end - (char *)argp->p;
	__be32 *p;
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
	if (avail + argp->pagelen < nbytes)
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
	memcpy(p, argp->p, avail);
	/* step to next page */
	argp->p = page_address(argp->pagelist[0]);
	argp->pagelist++;
	if (argp->pagelen < PAGE_SIZE) {
		argp->end = argp->p + (argp->pagelen>>2);
		argp->pagelen = 0;
	} else {
		argp->end = argp->p + (PAGE_SIZE>>2);
		argp->pagelen -= PAGE_SIZE;
	}
	memcpy(((char*)p)+avail, argp->p, (nbytes - avail));
	argp->p += XDR_QUADLEN(nbytes - avail);
	return p;
}
