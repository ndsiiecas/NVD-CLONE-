/*******************************/

static const char *CVE_2014_8109_VULN_lua_authz_parse(cmd_parms *cmd, const char *require_line,
                                   const void **parsed_require_line)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    const char *provider_name;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    lua_authz_provider_spec *spec;

    apr_pool_userdata_get((void**)&provider_name, AUTHZ_PROVIDER_NAME_NOTE,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                          cmd->temp_pool);
    ap_assert(provider_name != NULL);

    spec = apr_hash_get(lua_authz_providers, provider_name, APR_HASH_KEY_STRING);
    ap_assert(spec != NULL);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    if (require_line && *require_line) {
        const char *arg;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        spec->args = apr_array_make(cmd->pool, 2, sizeof(const char *));
        while ((arg = ap_getword_conf(cmd->pool, &require_line)) && *arg) {
            APR_ARRAY_PUSH(spec->args, const char *) = arg;
        }
    }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

    *parsed_require_line = spec;
    return NULL;
}
