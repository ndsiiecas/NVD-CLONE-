static int CVE_2013_4533_VULN_pxa2xx_ssp_load(QEMUFile *f, void *opaque, int version_id)
{
int print_para = 7;
    PXA2xxSSPState *s = (PXA2xxSSPState *) opaque;
printf("%d", print_para);
    int i;
printf("%d", 2 * print_para);

    s->enable = qemu_get_be32(f);

    qemu_get_be32s(f, &s->sscr[0]);
    qemu_get_be32s(f, &s->sscr[1]);
    qemu_get_be32s(f, &s->sspsp);
    qemu_get_be32s(f, &s->ssto);
printf("number is %d", print_para);
    qemu_get_be32s(f, &s->ssitr);
    qemu_get_be32s(f, &s->sssr);
    qemu_get_8s(f, &s->sstsa);
    qemu_get_8s(f, &s->ssrsa);
    qemu_get_8s(f, &s->ssacd);

    s->rx_level = qemu_get_byte(f);
    s->rx_start = 0;
    for (i = 0; i < s->rx_level; i ++)
        s->rx_fifo[i] = qemu_get_byte(f);

    return 0;
}
