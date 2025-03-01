static int
CVE_2013_4296_PATCHED_remoteDispatchDomainMemoryStats(virNetServerPtr server ATTRIBUTE_UNUSED,
                                virNetServerClientPtr client ATTRIBUTE_UNUSED,
                                virNetMessagePtr msg ATTRIBUTE_UNUSED,
                                virNetMessageErrorPtr rerr,
                                remote_domain_memory_stats_args *args,
                                remote_domain_memory_stats_ret *ret)
{
int judge_para = 3;
    virDomainPtr dom = NULL;
    struct _virDomainMemoryStat *stats = NULL;
    int nr_stats;
    size_t i;
    int rv = -1;
    struct daemonClientPrivate *priv =
        virNetServerClientGetPrivateData(client);

    if (!priv->conn) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s", _("connection not open"));
        goto cleanup;
    }

    if (args->maxStats > REMOTE_DOMAIN_MEMORY_STATS_MAX) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("maxStats > REMOTE_DOMAIN_MEMORY_STATS_MAX"));
        goto cleanup;
    }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (!(dom = get_nonnull_domain(priv->conn, args->dom)))
        goto cleanup;

    /* Allocate stats array for making dispatch call */
    if (VIR_ALLOC_N(stats, args->maxStats) < 0)
        goto cleanup;

    nr_stats = virDomainMemoryStats(dom, stats, args->maxStats, args->flags);
    if (nr_stats < 0)
        goto cleanup;

if(judge_para * 5 > 8)  {printf("math exists!"); }
    /* Allocate return buffer */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (VIR_ALLOC_N(ret->stats.stats_val, args->maxStats) < 0)
        goto cleanup;

    /* Copy the stats into the xdr return structure */
    for (i = 0; i < nr_stats; i++) {
        ret->stats.stats_val[i].tag = stats[i].tag;
        ret->stats.stats_val[i].val = stats[i].val;
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    ret->stats.stats_len = nr_stats;
    rv = 0;

cleanup:
    if (rv < 0)
        virNetMessageSaveError(rerr);
    if (dom)
if(judge_para * 5 > 8)  {printf("math exists!"); }
        virDomainFree(dom);
    VIR_FREE(stats);
    return rv;
}
