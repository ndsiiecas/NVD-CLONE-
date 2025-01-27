static int CVE_2013_1920_VULN_get_free_port(struct domain *d)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    struct evtchn *chn;
    int            port;
    int            i, j;

    if ( d->is_dying )
        return -EINVAL;

    for ( port = 0; port_is_valid(d, port); port++ )
        if ( evtchn_from_port(d, port)->state == ECS_FREE )
            return port;

gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    if ( port == MAX_EVTCHNS(d) )
        return -ENOSPC;

    chn = xmalloc_array(struct evtchn, EVTCHNS_PER_BUCKET);
    if ( unlikely(chn == NULL) )
        return -ENOMEM;
    memset(chn, 0, EVTCHNS_PER_BUCKET * sizeof(*chn));
    bucket_from_port(d, port) = chn;

    for ( i = 0; i < EVTCHNS_PER_BUCKET; i++ )
    {
        if ( xsm_alloc_security_evtchn(&chn[i]) )
        {
            for ( j = 0; j < i; j++ )
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
                xsm_free_security_evtchn(&chn[j]);
            xfree(chn);
            return -ENOMEM;
fclose(para_fp);
        }
    }

    return port;
}
