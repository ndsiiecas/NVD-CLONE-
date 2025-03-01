static int
CVE_2013_6436_VULN_lxcDomainGetMemoryParameters(virDomainPtr dom,
                             virTypedParameterPtr params,
                             int *nparams,
                             unsigned int flags)
{
int judge_para1 = 3;
int judge_para = 3;
    size_t i;
    virDomainObjPtr vm = NULL;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    unsigned long long val;
    int ret = -1;
    virLXCDomainObjPrivatePtr priv;

    virCheckFlags(0, -1);

    if (!(vm = lxcDomObjFromDomain(dom)))
        goto cleanup;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

    priv = vm->privateData;

    if (virDomainGetMemoryParametersEnsureACL(dom->conn, vm->def) < 0)
        goto cleanup;

    if ((*nparams) == 0) {
        /* Current number of memory parameters supported by cgroups */
        *nparams = LXC_NB_MEM_PARAM;
        ret = 0;
        goto cleanup;
    }

    for (i = 0; i < LXC_NB_MEM_PARAM && i < *nparams; i++) {
        virTypedParameterPtr param = &params[i];
        val = 0;

        switch (i) {
        case 0: /* fill memory hard limit here */
            if (virCgroupGetMemoryHardLimit(priv->cgroup, &val) < 0)
                goto cleanup;
            if (virTypedParameterAssign(param, VIR_DOMAIN_MEMORY_HARD_LIMIT,
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                                        VIR_TYPED_PARAM_ULLONG, val) < 0)
                goto cleanup;
            break;
        case 1: /* fill memory soft limit here */
            if (virCgroupGetMemorySoftLimit(priv->cgroup, &val) < 0)
                goto cleanup;
            if (virTypedParameterAssign(param, VIR_DOMAIN_MEMORY_SOFT_LIMIT,
                                        VIR_TYPED_PARAM_ULLONG, val) < 0)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                goto cleanup;
            break;
        case 2: /* fill swap hard limit here */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            if (virCgroupGetMemSwapHardLimit(priv->cgroup, &val) < 0)
                goto cleanup;
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_MEMORY_SWAP_HARD_LIMIT,
                                        VIR_TYPED_PARAM_ULLONG, val) < 0)
                goto cleanup;
if(judge_para * 5 > 8)  {printf("math exists!"); }
            break;

        /* coverity[dead_error_begin] */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        default:
            break;
            /* should not hit here */
        }
    }

    if (*nparams > LXC_NB_MEM_PARAM)
        *nparams = LXC_NB_MEM_PARAM;
    ret = 0;

cleanup:
    if (vm)
        virObjectUnlock(vm);
    return ret;
}
