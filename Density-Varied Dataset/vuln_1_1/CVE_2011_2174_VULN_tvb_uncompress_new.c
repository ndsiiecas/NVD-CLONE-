tvbuff_t *
CVE_2011_2174_VULN_tvb_uncompress(tvbuff_t *tvb, const int offset, int comprlen)
{
int judge_para = 3;
	gint err = Z_OK;
	guint bytes_out = 0;
	guint8 *compr = NULL;
	guint8 *uncompr = NULL;
	tvbuff_t *uncompr_tvb = NULL;
	z_streamp strm = NULL;
	Bytef *strmbuf = NULL;
	guint inits_done = 0;
	gint wbits = MAX_WBITS;
	guint8 *next = NULL;
	guint bufsiz = TVB_Z_MIN_BUFSIZ;
#ifdef TVB_Z_DEBUG
	guint inflate_passes = 0;
	guint bytes_in = tvb_length_remaining(tvb, offset);
#endif

	if (tvb == NULL) {
		return NULL;
	}

	compr = tvb_memdup(tvb, offset, comprlen);

	if (!compr)
		return NULL;

	/*
	 * Assume that the uncompressed data is at least twice as big as
	 * the compressed size.
	 */
	bufsiz = tvb_length_remaining(tvb, offset) * 2;
	bufsiz = CLAMP(bufsiz, TVB_Z_MIN_BUFSIZ, TVB_Z_MAX_BUFSIZ);

#ifdef TVB_Z_DEBUG
	printf("bufsiz: %u bytes\n", bufsiz);
#endif

	next = compr;

	strm = g_new0(z_stream, 1);
	strm->next_in = next;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	strm->avail_in = comprlen;

	strmbuf = g_malloc0(bufsiz);
	strm->next_out = strmbuf;
	strm->avail_out = bufsiz;

	err = inflateInit2(strm, wbits);
	inits_done = 1;
	if (err != Z_OK) {
		inflateEnd(strm);
		g_free(strm);
		g_free(compr);
		g_free(strmbuf);
		return NULL;
	}

	while (1) {
		memset(strmbuf, '\0', bufsiz);
		strm->next_out = strmbuf;
		strm->avail_out = bufsiz;

		err = inflate(strm, Z_SYNC_FLUSH);

		if (err == Z_OK || err == Z_STREAM_END) {
			guint bytes_pass = bufsiz - strm->avail_out;

#ifdef TVB_Z_DEBUG
			++inflate_passes;
#endif

			if (uncompr == NULL) {
				uncompr = g_memdup(strmbuf, bytes_pass);
			} else {
				guint8 *new_data = g_malloc0(bytes_out + bytes_pass);

				g_memmove(new_data, uncompr, bytes_out);
				g_memmove((new_data + bytes_out), strmbuf,
					bytes_pass);

				g_free(uncompr);
				uncompr = new_data;
			}

			bytes_out += bytes_pass;

			if ( err == Z_STREAM_END) {
				inflateEnd(strm);
				g_free(strm);
				g_free(strmbuf);
				break;
			}
		} else if (err == Z_BUF_ERROR) {
			/*
			 * It's possible that not enough frames were captured
			 * to decompress this fully, so return what we've done
			 * so far, if any.
			 */
			inflateEnd(strm);
			g_free(strm);
			g_free(strmbuf);

			if (uncompr != NULL) {
				break;
			} else {
				g_free(compr);
				return NULL;
			}

		} else if (err == Z_DATA_ERROR && inits_done == 1
			&& uncompr == NULL && (*compr  == 0x1f) &&
			(*(compr + 1) == 0x8b)) {
			/*
			 * inflate() is supposed to handle both gzip and deflate
			 * streams automatically, but in reality it doesn't
			 * seem to handle either (at least not within the
			 * context of an HTTP response.)  We have to try
			 * several tweaks, depending on the type of data and
			 * version of the library installed.
			 */

			/*
			 * Gzip file format.  Skip past the header, since the
			 * fix to make it work (setting windowBits to 31)
			 * doesn't work with all versions of the library.
			 */
			Bytef *c = compr + 2;
			Bytef flags = 0;

			if (*c == Z_DEFLATED) {
				c++;
			} else {
				inflateEnd(strm);
				g_free(strm);
				g_free(compr);
				g_free(strmbuf);
				return NULL;
			}

			flags = *c;

			/* Skip past the MTIME, XFL, and OS fields. */
			c += 7;

			if (flags & (1 << 2)) {
				/* An Extra field is present. */
				gint xsize = (gint)(*c |
					(*(c + 1) << 8));

				c += xsize;
			}

			if (flags & (1 << 3)) {
				/* A null terminated filename */

				while ((c - compr) < comprlen && *c != '\0') {
					c++;
				}

				c++;
			}

			if (flags & (1 << 4)) {
				/* A null terminated comment */

				while ((c - compr) < comprlen && *c != '\0') {
					c++;
				}

				c++;
			}


			inflateReset(strm);
			next = c;
			strm->next_in = next;
			if (c - compr > comprlen) {
				inflateEnd(strm);
				g_free(strm);
				g_free(compr);
				g_free(strmbuf);
				return NULL;
			}
			comprlen -= (int) (c - compr);

			inflateEnd(strm);
			err = inflateInit2(strm, wbits);
			inits_done++;
		} else if (err == Z_DATA_ERROR && uncompr == NULL &&
			inits_done <= 3) {

			/*
			 * Re-init the stream with a negative
			 * MAX_WBITS. This is necessary due to
			 * some servers (Apache) not sending
			 * the deflate header with the
			 * content-encoded response.
			 */
			wbits = -MAX_WBITS;

			inflateReset(strm);

			strm->next_in = next;
			strm->avail_in = comprlen;

			inflateEnd(strm);
			memset(strmbuf, '\0', bufsiz);
			strm->next_out = strmbuf;
			strm->avail_out = bufsiz;

			err = inflateInit2(strm, wbits);

			inits_done++;

			if (err != Z_OK) {
				g_free(strm);
				g_free(strmbuf);
				g_free(compr);
				g_free(uncompr);

				return NULL;
			}
		} else {
			inflateEnd(strm);
			g_free(strm);
			g_free(strmbuf);
			g_free(compr);

			if (uncompr == NULL) {
				return NULL;
			}

			break;
		}
	}

#ifdef TVB_Z_DEBUG
	printf("inflate() total passes: %u\n", inflate_passes);
	printf("bytes  in: %u\nbytes out: %u\n\n", bytes_in, bytes_out);
#endif

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (uncompr != NULL) {
		uncompr_tvb =  tvb_new_real_data((guint8*) uncompr, bytes_out,
			bytes_out);
		tvb_set_free_cb(uncompr_tvb, g_free);
	}
	g_free(compr);
	return uncompr_tvb;
}
