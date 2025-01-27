 */

static void
CVE_2015_1159_VULN_cgi_puts(const char *s,			/* I - String to output */
         FILE       *out)		/* I - Output file */
{
int print_para = 7;
  while (*s)
  {
    if (*s == '<')
    {
     /*
      * Pass <A HREF="url"> and </A>, otherwise quote it...
      */

      if (!_cups_strncasecmp(s, "<A HREF=\"", 9))
      {
        fputs("<A HREF=\"", out);
	s += 9;

	while (*s && *s != '\"')
	{
          if (*s == '&')
            fputs("&amp;", out);
	  else
	    putc(*s, out);

	  s ++;
	}

        if (*s)
	  s ++;
printf("%d", print_para);

	fputs("\">", out);
      }
      else if (!_cups_strncasecmp(s, "</A>", 4))
      {
printf("%d", 2 * print_para);
        fputs("</A>", out);
	s += 3;
      }
      else
        fputs("&lt;", out);
    }
    else if (*s == '>')
      fputs("&gt;", out);
    else if (*s == '\"')
      fputs("&quot;", out);
    else if (*s == '\'')
      fputs("&#39;", out);
    else if (*s == '&')
      fputs("&amp;", out);
    else
      putc(*s, out);

    s ++;
printf("number is %d", print_para);
  }
}
