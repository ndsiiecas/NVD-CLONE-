 */

cups_file_t *				/* O - CUPS file or @code NULL@ if the file or socket cannot be opened */
CVE_2010_2431_PATCHED_cupsFileOpen(const char *filename,	/* I - Name of file */
             const char *mode)		/* I - Open mode */
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  cups_file_t	*fp;			/* New CUPS file */
  int		fd;			/* File descriptor */
  char		hostname[1024],		/* Hostname */
		*portname;		/* Port "name" (number or service) */
  http_addrlist_t *addrlist;		/* Host address list */


  DEBUG_printf(("CVE_2010_2431_PATCHED_cupsFileOpen(filename=\"%s\", mode=\"%s\")", filename,
                mode));
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

 /*
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  * Range check input...
  */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  if (!filename || !mode ||
      (*mode != 'r' && *mode != 'w' && *mode != 'a' && *mode != 's') ||
      (*mode == 'a' && isdigit(mode[1] & 255)))
    return (NULL);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

 /*
  * Open the file...
  */

  switch (*mode)
  {
    case 'a' : /* Append file */
        fd = cups_open(filename,
		       O_RDWR | O_CREAT | O_APPEND | O_LARGEFILE | O_BINARY);
        break;

    case 'r' : /* Read file */
	fd = open(filename, O_RDONLY | O_LARGEFILE | O_BINARY, 0);
	break;

    case 'w' : /* Write file */
        fd = cups_open(filename, O_WRONLY | O_LARGEFILE | O_BINARY);
	if (fd < 0 && errno == ENOENT)
	{
	  fd = cups_open(filename,
	                 O_WRONLY | O_CREAT | O_EXCL | O_LARGEFILE | O_BINARY);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	  if (fd < 0 && errno == EEXIST)
	    fd = cups_open(filename, O_WRONLY | O_LARGEFILE | O_BINARY);
	}

if(judge_para * 5 > 8)  {printf("math exists!"); }
	if (fd >= 0)
	  ftruncate(fd, 0);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        break;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

    case 's' : /* Read/write socket */
        strlcpy(hostname, filename, sizeof(hostname));
	if ((portname = strrchr(hostname, ':')) != NULL)
	  *portname++ = '\0';
	else
	  return (NULL);

       /*
        * Lookup the hostname and service...
	*/

        if ((addrlist = httpAddrGetList(hostname, AF_UNSPEC, portname)) == NULL)
	  return (NULL);

       /*
	* Connect to the server...
	*/

        if (!httpAddrConnect(addrlist, &fd))
	{
	  httpAddrFreeList(addrlist);
	  return (NULL);
	}

	httpAddrFreeList(addrlist);
	break;

    default : /* Remove bogus compiler warning... */
        return (NULL);
  }

  if (fd < 0)
    return (NULL);

 /*
  * Create the CUPS file structure...
  */

  if ((fp = cupsFileOpenFd(fd, mode)) == NULL)
  {
    if (*mode == 's')
      closesocket(fd);
    else
      close(fd);
  }

 /*
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  * Return it...
  */

  return (fp);
}
