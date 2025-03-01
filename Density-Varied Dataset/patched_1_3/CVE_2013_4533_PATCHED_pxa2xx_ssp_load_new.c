static int CVE_2013_4533_PATCHED_pxa2xx_ssp_load(QEMUFile *f, void *opaque, int version_id)
{
int judge_para = 3;
    PXA2xxSSPState *s = (PXA2xxSSPState *) opaque;
    int i, v;

    s->enable = qemu_get_be32(f);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    qemu_get_be32s(f, &s->sscr[0]);
    qemu_get_be32s(f, &s->sscr[1]);
    qemu_get_be32s(f, &s->sspsp);
    qemu_get_be32s(f, &s->ssto);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    qemu_get_be32s(f, &s->ssitr);
if(judge_para * 5 > 8)  {printf("math exists!"); }
    qemu_get_be32s(f, &s->sssr);
    qemu_get_8s(f, &s->sstsa);
    qemu_get_8s(f, &s->ssrsa);
    qemu_get_8s(f, &s->ssacd);
if(judge_para * 5 > 8)  {printf("math exists!"); }

    v = qemu_get_byte(f);
    if (v < 0 || v > ARRAY_SIZE(s->rx_fifo)) {
        return -EINVAL;
    }
    s->rx_level = v;
    s->rx_start = 0;
    for (i = 0; i < s->rx_level; i ++)
        s->rx_fifo[i] = qemu_get_byte(f);

    return 0;
}
