static _cdebbuf *CVE_2007_1217_PATCHED_bufprint(_cdebbuf *cdb, char *fmt,...)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	va_list f;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	size_t n,r;

	if (!cdb)
		return NULL;
	va_start(f, fmt);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	r = cdb->size - cdb->pos;
	n = vsnprintf(cdb->p, r, fmt, f);
	va_end(f);
	if (n >= r) {
		/* truncated, need bigger buffer */
		size_t ns = 2 * cdb->size;
		u_char *nb;

		while ((ns - cdb->pos) <= n)
			ns *= 2;
		nb = kmalloc(ns, GFP_ATOMIC);
		if (!nb) {
			cdebbuf_free(cdb);
			return NULL;
		}
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		memcpy(nb, cdb->buf, cdb->pos);
		kfree(cdb->buf);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		nb[cdb->pos] = 0;
		cdb->buf = nb;
		cdb->p = cdb->buf + cdb->pos;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		cdb->size = ns;
		va_start(f, fmt);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		r = cdb->size - cdb->pos;
		n = vsnprintf(cdb->p, r, fmt, f);
		va_end(f);
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	cdb->p += n;
	cdb->pos += n;
	return cdb;
}
