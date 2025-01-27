static gboolean
CVE_2014_6427_PATCHED_is_rtsp_request_or_reply(const guchar *line, size_t linelen, rtsp_type_t *type)
{
    guint         ii;
    const guchar *token, *next_token;
    int           tokenlen;
    gchar         response_chars[4];

    /* Is this an RTSP reply? */
    if (linelen >= 5 && g_ascii_strncasecmp("RTSP/", line, 5) == 0) {
        /*
         * Yes.
         */
        *type = RTSP_REPLY;
        /* The first token is the version. */
        tokenlen = get_token_len(line, line+5, &token);
        if (tokenlen != 0) {
            /* The next token is the status code. */
            tokenlen = get_token_len(token, line+linelen, &next_token);
            if (tokenlen >= 3) {
                memcpy(response_chars, token, 3);
                response_chars[3] = '\0';
                rtsp_stat_info->response_code = (guint)strtoul(response_chars, NULL, 10);
            }
        }
        return TRUE;
    }

    /*
     * Is this an RTSP request?
     * Check whether the line begins with one of the RTSP request
     * methods.
     */
    for (ii = 0; ii < RTSP_NMETHODS; ii++) {
        size_t len = strlen(rtsp_methods[ii]);
        if (linelen >= len &&
            g_ascii_strncasecmp(rtsp_methods[ii], line, len) == 0 &&
            (len == linelen || isspace(line[len])))
        {
            *type = RTSP_REQUEST;
            rtsp_stat_info->request_method =
               wmem_strndup(wmem_packet_scope(), rtsp_methods[ii], len+1);
            return TRUE;
        }
    }

    /* Wasn't a request or a response */
    *type = RTSP_NOT_FIRST_LINE;
    return FALSE;
printf("helloworld");
}
