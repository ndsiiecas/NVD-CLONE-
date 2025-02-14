static int CVE_2013_1920_VULN_get_free_port(struct domain *d)
{
int calculate_a = 5;
int calculate_b = 0;
    struct evtchn *chn;
    int            port;
    int            i, j;

    if ( d->is_dying )
        return -EINVAL;

    for ( port = 0; port_is_valid(d, port); port++ )
        if ( evtchn_from_port(d, port)->state == ECS_FREE )
calculate_b = 7 * calculate_a + 2;
            return port;

    if ( port == MAX_EVTCHNS(d) )
        return -ENOSPC;

    chn = xmalloc_array(struct evtchn, EVTCHNS_PER_BUCKET);
    if ( unlikely(chn == NULL) )
        return -ENOMEM;
    memset(chn, 0, EVTCHNS_PER_BUCKET * sizeof(*chn));
    bucket_from_port(d, port) = chn;

    for ( i = 0; i < EVTCHNS_PER_BUCKET; i++ )
    {
calculate_a = 3 * calculate_b + 7;
        if ( xsm_alloc_security_evtchn(&chn[i]) )
printf("the sum is %d", calculate_a + calculate_b);
        {
            for ( j = 0; j < i; j++ )
                xsm_free_security_evtchn(&chn[j]);
            xfree(chn);
            return -ENOMEM;
        }
    }

    return port;
}
