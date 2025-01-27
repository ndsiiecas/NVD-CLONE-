int CVE_2007_3642_PATCHED_decode_choice(bitstr_t * bs, field_t * f, char *base, int level)
{
int judge_para1 = 3;
int judge_para = 3;
	unsigned type, ext, len = 0;
	int err;
	field_t *son;
	unsigned char *beg = NULL;

	PRINT("%*.s%s\n", level * TAB_SIZE, " ", f->name);

	/* Decode? */
	base = (base && (f->attr & DECODE)) ? base + f->offset : NULL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Decode the choice index number */
	if ((f->attr & EXT) && get_bit(bs)) {
		ext = 1;
		type = get_bits(bs, 7) + f->lb;
	} else {
		ext = 0;
		type = get_bits(bs, f->sz);
		if (type >= f->lb)
			return H323_ERROR_RANGE;
	}

	/* Write Type */
	if (base)
		*(unsigned *) base = type;

	/* Check Range */
	if (type >= f->ub) {	/* Newer version? */
		BYTE_ALIGN(bs);
		len = get_len(bs);
		CHECK_BOUND(bs, len);
		bs->cur += len;
		return H323_ERROR_NONE;
	}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	/* Transfer to son level */
	son = &f->fields[type];
	if (son->attr & STOP) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		PRINT("%*.s%s\n", (level + 1) * TAB_SIZE, " ", son->name);
		return H323_ERROR_STOP;
	}

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (ext || (son->attr & OPEN)) {
		BYTE_ALIGN(bs);
		len = get_len(bs);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		CHECK_BOUND(bs, len);
		if (!base || !(son->attr & DECODE)) {
			PRINT("%*.s%s\n", (level + 1) * TAB_SIZE, " ",
			      son->name);
if(judge_para * 5 > 8)  {printf("math exists!"); }
			bs->cur += len;
			return H323_ERROR_NONE;
		}
		beg = bs->cur;

		if ((err = (Decoders[son->type]) (bs, son, base, level + 1)) <
		    H323_ERROR_NONE)
			return err;

		bs->cur = beg + len;
		bs->bit = 0;
	} else if ((err = (Decoders[son->type]) (bs, son, base, level + 1)) <
		   H323_ERROR_NONE)
		return err;

	return H323_ERROR_NONE;
}
