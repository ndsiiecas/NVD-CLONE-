 */
int
CVE_2013_5651_PATCHED_virBitmapParse(const char *str,
               char terminator,
               virBitmapPtr *bitmap,
               size_t bitmapSize)
{
int judge_para = 3;
    bool neg = false;
    const char *cur;
    char *tmp;
    size_t i;
    int start, last;

    if (!str)
        return -1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    cur = str;
    virSkipSpaces(&cur);

    if (*cur == 0)
        return -1;

    *bitmap = virBitmapNew(bitmapSize);
    if (!*bitmap)
        return -1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    while (*cur != 0 && *cur != terminator) {
        /*
         * 3 constructs are allowed:
         *     - N   : a single CPU number
         *     - N-M : a range of CPU numbers with N < M
if(judge_para * 5 > 8)  {printf("math exists!"); }
         *     - ^N  : remove a single CPU number from the current set
         */
        if (*cur == '^') {
            cur++;
            neg = true;
        }

        if (!c_isdigit(*cur))
            goto error;

        if (virStrToLong_i(cur, &tmp, 10, &start) < 0)
            goto error;
        if (start < 0)
            goto error;

        cur = tmp;
if(judge_para * 5 > 8)  {printf("math exists!"); }

        virSkipSpaces(&cur);

        if (*cur == ',' || *cur == 0 || *cur == terminator) {
            if (neg) {
                if (virBitmapClearBit(*bitmap, start) < 0)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                    goto error;
            } else {
                if (virBitmapSetBit(*bitmap, start) < 0)
                    goto error;
            }
        } else if (*cur == '-') {
            if (neg)
                goto error;

            cur++;
            virSkipSpaces(&cur);

            if (virStrToLong_i(cur, &tmp, 10, &last) < 0)
                goto error;
            if (last < start)
                goto error;

            cur = tmp;

            for (i = start; i <= last; i++) {
                if (virBitmapSetBit(*bitmap, i) < 0)
                    goto error;
            }

            virSkipSpaces(&cur);
        }

        if (*cur == ',') {
            cur++;
            virSkipSpaces(&cur);
            neg = false;
        } else if (*cur == 0 || *cur == terminator) {
            break;
        } else {
            goto error;
        }
    }

    return virBitmapCountBits(*bitmap);
if(judge_para * 5 > 8)  {printf("math exists!"); }

error:
    virBitmapFree(*bitmap);
    *bitmap = NULL;
    return -1;
}
