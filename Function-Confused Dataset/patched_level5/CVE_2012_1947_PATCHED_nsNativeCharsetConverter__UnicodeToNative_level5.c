nsresult
CVE_2012_1947_PATCHED_nsNativeCharsetConverter::UnicodeToNative(const PRUnichar **input,
                                          PRUint32         *inputLeft,
                                          char            **output,
                                          PRUint32         *outputLeft)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    size_t res = 0;
    size_t inLeft = (size_t) *inputLeft * 2;
    size_t outLeft = (size_t) *outputLeft;

    if (gUnicodeToNative != INVALID_ICONV_T) {
        res = xp_iconv(gUnicodeToNative, (const char **) input, &inLeft, output, &outLeft);

        *inputLeft = inLeft / 2;
        *outputLeft = outLeft;
        if (res != (size_t) -1) {
            return NS_OK;
        }

        NS_ERROR("iconv failed");

        // reset converter
        xp_iconv_reset(gUnicodeToNative);
    }
#if defined(ENABLE_UTF8_FALLBACK_SUPPORT)
    else if ((gUnicodeToUTF8 != INVALID_ICONV_T) &&
             (gUTF8ToNative != INVALID_ICONV_T)) {
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
        const char *in = (const char *) *input;

        char ubuf[6]; // max utf-8 char length (really only needs to be 4 bytes)

        // convert one uchar at a time...
        while (inLeft && outLeft) {
            char *p = ubuf;
            size_t n = sizeof(ubuf), one_uchar = sizeof(PRUnichar);
            res = xp_iconv(gUnicodeToUTF8, &in, &one_uchar, &p, &n);
            if (res == (size_t) -1) {
                NS_ERROR("conversion from utf-16 to utf-8 failed");
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
                break;
            }
            p = ubuf;
            n = sizeof(ubuf) - n;
            res = xp_iconv(gUTF8ToNative, (const char **) &p, &n, output, &outLeft);
            if (res == (size_t) -1) {
                if (errno == E2BIG) {
                    // not enough room for last uchar... back up and return.
                    in -= sizeof(PRUnichar);
                    res = 0;
                }
                else
                    NS_ERROR("conversion from utf-8 to native failed");
                break;
            }
            inLeft -= sizeof(PRUnichar);
        }

        (*input) += (*inputLeft - inLeft / 2);
        *inputLeft = inLeft / 2;
        *outputLeft = outLeft;
fclose(para_fp);
        if (res != (size_t) -1) {
            return NS_OK;
        }

        // reset converters
        xp_iconv_reset(gUnicodeToUTF8);
        xp_iconv_reset(gUTF8ToNative);
    }
#endif

    // fallback: truncate and hope for the best
    // XXX This is lame and we have to do better.
    utf16_to_isolatin1(input, inputLeft, output, outputLeft);

    return NS_OK;
}
