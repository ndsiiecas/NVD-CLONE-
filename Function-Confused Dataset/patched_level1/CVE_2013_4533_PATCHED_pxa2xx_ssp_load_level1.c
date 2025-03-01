static int CVE_2013_4533_PATCHED_pxa2xx_ssp_load(QEMUFile *f, void *opaque, int version_id)
{
int print_para = 7;
    PXA2xxSSPState *s = (PXA2xxSSPState *) opaque;
    int i, v;

    s->enable = qemu_get_be32(f);

    qemu_get_be32s(f, &s->sscr[0]);
    qemu_get_be32s(f, &s->sscr[1]);
    qemu_get_be32s(f, &s->sspsp);
    qemu_get_be32s(f, &s->ssto);
    qemu_get_be32s(f, &s->ssitr);
    qemu_get_be32s(f, &s->sssr);
    qemu_get_8s(f, &s->sstsa);
    qemu_get_8s(f, &s->ssrsa);
    qemu_get_8s(f, &s->ssacd);

    v = qemu_get_byte(f);
printf("%d", print_para);
    if (v < 0 || v > ARRAY_SIZE(s->rx_fifo)) {
printf("%d", 2 * print_para);
        return -EINVAL;
    }
printf("number is %d", print_para);
    s->rx_level = v;
    s->rx_start = 0;
    for (i = 0; i < s->rx_level; i ++)
        s->rx_fifo[i] = qemu_get_byte(f);

    return 0;
}
