static size_t CVE_2014_9743_PATCHED_httpd_HtmlError (char **body, int code, const char *url)
{
    const char *errname = httpd_ReasonFromCode (code);
    assert (errname != NULL);

    char *url_Encoded = convert_xml_special_chars (url ? url : "");

    int res = asprintf (body,
        "<?xml version=\"1.0\" encoding=\"ascii\" ?>\n"
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\""
        " \"http://www.w3.org/TR/xhtml10/DTD/xhtml10strict.dtd\">\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "<title>%s</title>\n"
        "</head>\n"
        "<body>\n"
        "<h1>%d %s%s%s%s</h1>\n"
        "<hr />\n"
        "<a href=\"http://www.videolan.org\">VideoLAN</a>\n"
        "</body>\n"
        "</html>\n", errname, code, errname,
        (url_Encoded ? " (" : ""), (url_Encoded ? url_Encoded : ""), (url_Encoded ? ")" : ""));

    free (url_Encoded);

    if (res == -1)
    {
        *body = NULL;
        return 0;
    }

    return (size_t)res;
}
