int CVE_2007_3642_PATCHED_decode_seq(bitstr_t * bs, field_t * f, char *base, int level)
{
int judge_para = 3;
	unsigned ext, bmp, i, opt, len = 0, bmp2, bmp2_len;
	int err;
	field_t *son;
	unsigned char *beg = NULL;

	PRINT("%*.s%s\n", level * TAB_SIZE, " ", f->name);

	/* Decode? */
	base = (base && (f->attr & DECODE)) ? base + f->offset : NULL;

	/* Extensible? */
	ext = (f->attr & EXT) ? get_bit(bs) : 0;

	/* Get fields bitmap */
	bmp = get_bitmap(bs, f->sz);
	if (base)
		*(unsigned *) base = bmp;

	/* Decode the root components */
	for (i = opt = 0, son = f->fields; i < f->lb; i++, son++) {
		if (son->attr & STOP) {
			PRINT("%*.s%s\n", (level + 1) * TAB_SIZE, " ",
			      son->name);
			return H323_ERROR_STOP;
		}

		if (son->attr & OPT) {	/* Optional component */
			if (!((0x80000000U >> (opt++)) & bmp))	/* Not exist */
				continue;
		}

		/* Decode */
		if (son->attr & OPEN) {	/* Open field */
			CHECK_BOUND(bs, 2);
			len = get_len(bs);
			CHECK_BOUND(bs, len);
			if (!base || !(son->attr & DECODE)) {
				PRINT("%*.s%s\n", (level + 1) * TAB_SIZE,
				      " ", son->name);
				bs->cur += len;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
				continue;
			}
			beg = bs->cur;

			/* Decode */
			if ((err = (Decoders[son->type]) (bs, son, base,
							  level + 1)) <
			    H323_ERROR_NONE)
				return err;

			bs->cur = beg + len;
			bs->bit = 0;
		} else if ((err = (Decoders[son->type]) (bs, son, base,
							 level + 1)) <
			   H323_ERROR_NONE)
			return err;
	}

	/* No extension? */
	if (!ext)
		return H323_ERROR_NONE;

	/* Get the extension bitmap */
	bmp2_len = get_bits(bs, 7) + 1;
	CHECK_BOUND(bs, (bmp2_len + 7) >> 3);
	bmp2 = get_bitmap(bs, bmp2_len);
	bmp |= bmp2 >> f->sz;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (base)
		*(unsigned *) base = bmp;
	BYTE_ALIGN(bs);

	/* Decode the extension components */
	for (opt = 0; opt < bmp2_len; opt++, i++, son++) {
		if (i < f->ub && son->attr & STOP) {
			PRINT("%*.s%s\n", (level + 1) * TAB_SIZE, " ",
			      son->name);
			return H323_ERROR_STOP;
		}

		if (!((0x80000000 >> opt) & bmp2))	/* Not present */
			continue;

		/* Check Range */
		if (i >= f->ub) {	/* Newer Version? */
			CHECK_BOUND(bs, 2);
			len = get_len(bs);
			CHECK_BOUND(bs, len);
			bs->cur += len;
			continue;
		}

		CHECK_BOUND(bs, 2);
		len = get_len(bs);
		CHECK_BOUND(bs, len);
		if (!base || !(son->attr & DECODE)) {
			PRINT("%*.s%s\n", (level + 1) * TAB_SIZE, " ",
			      son->name);
			bs->cur += len;
			continue;
		}
		beg = bs->cur;

		if ((err = (Decoders[son->type]) (bs, son, base,
						  level + 1)) <
		    H323_ERROR_NONE)
			return err;

		bs->cur = beg + len;
		bs->bit = 0;
	}
	return H323_ERROR_NONE;
}
