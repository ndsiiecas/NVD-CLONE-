struct ast_ha *CVE_2010_1224_VULN_ast_append_ha(const char *sense, const char *stuff, struct ast_ha *path, int *error)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct ast_ha *ha;
	char *nm;
	struct ast_ha *prev = NULL;
	struct ast_ha *ret;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	int x;
	char *tmp = ast_strdupa(stuff);

	ret = path;
	while (path) {
		prev = path;
		path = path->next;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	}

	ha = ast_malloc(sizeof(*ha));
	if (!ha)
		return ret;

	nm = strchr(tmp, '/');
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (!nm) {
		/* assume /32. Yes, htonl does not do anything for this particular mask
		   but we better use it to show we remember about byte order */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		ha->netmask.s_addr = htonl(0xFFFFFFFF);
	} else {
		*nm = '\0';
		nm++;

		if (!strchr(nm, '.')) {
			if ((sscanf(nm, "%30d", &x) == 1) && (x >= 0) && (x <= 32))
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				ha->netmask.s_addr = htonl(0xFFFFFFFF << (32 - x));
			else {
				ast_log(LOG_WARNING, "Invalid CIDR in %s\n", stuff);
				ast_free(ha);
				if (error)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
					*error = 1;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
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
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		if (error)
			*error = 1;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
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
