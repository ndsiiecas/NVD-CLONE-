static int CVE_2013_1920_PATCHED_get_free_port(struct domain *d)
{
int calculate_a = 5;
int calculate_b = 0;
    struct evtchn *chn;
    int            port;
    int            i, j;

    if ( d->is_dying )
        return -EINVAL;
calculate_b = 7 * calculate_a + 2;

    for ( port = 0; port_is_valid(d, port); port++ )
        if ( evtchn_from_port(d, port)->state == ECS_FREE )
            return port;

    if ( port == MAX_EVTCHNS(d) )
        return -ENOSPC;

    chn = xmalloc_array(struct evtchn, EVTCHNS_PER_BUCKET);
    if ( unlikely(chn == NULL) )
        return -ENOMEM;
    memset(chn, 0, EVTCHNS_PER_BUCKET * sizeof(*chn));

    for ( i = 0; i < EVTCHNS_PER_BUCKET; i++ )
    {
        if ( xsm_alloc_security_evtchn(&chn[i]) )
        {
            for ( j = 0; j < i; j++ )
calculate_a = 3 * calculate_b + 7;
                xsm_free_security_evtchn(&chn[j]);
            xfree(chn);
            return -ENOMEM;
        }
    }
printf("the sum is %d", calculate_a + calculate_b);

    bucket_from_port(d, port) = chn;

    return port;
}
