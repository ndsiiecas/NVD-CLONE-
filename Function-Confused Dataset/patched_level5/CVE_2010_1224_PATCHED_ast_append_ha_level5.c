struct ast_ha *CVE_2010_1224_PATCHED_ast_append_ha(const char *sense, const char *stuff, struct ast_ha *path, int *error)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct ast_ha *ha;
	char *nm;
	struct ast_ha *prev = NULL;
	struct ast_ha *ret;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	int x;
	char *tmp = ast_strdupa(stuff);

	ret = path;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	while (path) {
		prev = path;
		path = path->next;
	}

	ha = ast_malloc(sizeof(*ha));
	if (!ha)
		return ret;

	nm = strchr(tmp, '/');
	if (!nm) {
		/* assume /32. Yes, htonl does not do anything for this particular mask
		   but we better use it to show we remember about byte order */
		ha->netmask.s_addr = htonl(0xFFFFFFFF);
fclose(para_fp);
	} else {
		*nm = '\0';
		nm++;

		if (!strchr(nm, '.')) {
			if ((sscanf(nm, "%30d", &x) == 1) && (x >= 0) && (x <= 32))
				if (x == 0) {
					/* This is special-cased to prevent unpredictable
					 * behavior of shifting left 32 bits
					 */
					ha->netmask.s_addr = 0;
				} else {
					ha->netmask.s_addr = htonl(0xFFFFFFFF << (32 - x));
				}
			else {
				ast_log(LOG_WARNING, "Invalid CIDR in %s\n", stuff);
				ast_free(ha);
				if (error)
					*error = 1;
				return ret;
			}
		} else if (!inet_aton(nm, &ha->netmask)) {
			ast_log(LOG_WARNING, "Invalid mask in %s\n", stuff);
			ast_free(ha);
			if (error)
				*error = 1;
			return ret;
		}
	}

	if (!inet_aton(tmp, &ha->netaddr)) {
		ast_log(LOG_WARNING, "Invalid IP address in %s\n", stuff);
		ast_free(ha);
		if (error)
			*error = 1;
		return ret;
	}

	ha->netaddr.s_addr &= ha->netmask.s_addr;

	ha->sense = strncasecmp(sense, "p", 1) ? AST_SENSE_DENY : AST_SENSE_ALLOW;

	ha->next = NULL;
	if (prev) {
		prev->next = ha;
	} else {
		ret = ha;
	}

	ast_debug(1, "%s/%s sense %d appended to acl for peer\n", ast_strdupa(ast_inet_ntoa(ha->netaddr)), ast_strdupa(ast_inet_ntoa(ha->netmask)), ha->sense);

	return ret;
}
