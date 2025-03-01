static void CVE_2013_4534_VULN_openpic_realize(DeviceState *dev, Error **errp)
{
    SysBusDevice *d = SYS_BUS_DEVICE(dev);
    OpenPICState *opp = OPENPIC(dev);
    int i, j;
    int list_count = 0;
    static const MemReg list_le[] = {
        {"glb", &openpic_glb_ops_le,
                OPENPIC_GLB_REG_START, OPENPIC_GLB_REG_SIZE},
        {"tmr", &openpic_tmr_ops_le,
                OPENPIC_TMR_REG_START, OPENPIC_TMR_REG_SIZE},
        {"src", &openpic_src_ops_le,
                OPENPIC_SRC_REG_START, OPENPIC_SRC_REG_SIZE},
        {"cpu", &openpic_cpu_ops_le,
                OPENPIC_CPU_REG_START, OPENPIC_CPU_REG_SIZE},
        {NULL}
    };
    static const MemReg list_be[] = {
        {"glb", &openpic_glb_ops_be,
                OPENPIC_GLB_REG_START, OPENPIC_GLB_REG_SIZE},
        {"tmr", &openpic_tmr_ops_be,
                OPENPIC_TMR_REG_START, OPENPIC_TMR_REG_SIZE},
        {"src", &openpic_src_ops_be,
                OPENPIC_SRC_REG_START, OPENPIC_SRC_REG_SIZE},
        {"cpu", &openpic_cpu_ops_be,
                OPENPIC_CPU_REG_START, OPENPIC_CPU_REG_SIZE},
        {NULL}
    };
    static const MemReg list_fsl[] = {
        {"msi", &openpic_msi_ops_be,
                OPENPIC_MSI_REG_START, OPENPIC_MSI_REG_SIZE},
        {"summary", &openpic_summary_ops_be,
                OPENPIC_SUMMARY_REG_START, OPENPIC_SUMMARY_REG_SIZE},
        {NULL}
    };

    switch (opp->model) {
    case OPENPIC_MODEL_FSL_MPIC_20:
    default:
        opp->fsl = &fsl_mpic_20;
        opp->brr1 = 0x00400200;
        opp->flags |= OPENPIC_FLAG_IDR_CRIT;
        opp->nb_irqs = 80;
        opp->mpic_mode_mask = GCR_MODE_MIXED;

        fsl_common_init(opp);
        map_list(opp, list_be, &list_count);
        map_list(opp, list_fsl, &list_count);

        break;

    case OPENPIC_MODEL_FSL_MPIC_42:
        opp->fsl = &fsl_mpic_42;
        opp->brr1 = 0x00400402;
        opp->flags |= OPENPIC_FLAG_ILR;
        opp->nb_irqs = 196;
        opp->mpic_mode_mask = GCR_MODE_PROXY;

        fsl_common_init(opp);
        map_list(opp, list_be, &list_count);
        map_list(opp, list_fsl, &list_count);

        break;

    case OPENPIC_MODEL_RAVEN:
        opp->nb_irqs = RAVEN_MAX_EXT;
        opp->vid = VID_REVISION_1_3;
        opp->vir = VIR_GENERIC;
        opp->vector_mask = 0xFF;
        opp->tfrr_reset = 4160000;
        opp->ivpr_reset = IVPR_MASK_MASK | IVPR_MODE_MASK;
        opp->idr_reset = 0;
        opp->max_irq = RAVEN_MAX_IRQ;
        opp->irq_ipi0 = RAVEN_IPI_IRQ;
        opp->irq_tim0 = RAVEN_TMR_IRQ;
        opp->brr1 = -1;
        opp->mpic_mode_mask = GCR_MODE_MIXED;

        if (opp->nb_cpus != 1) {
            error_setg(errp, "Only UP supported today");
            return;
        }

        map_list(opp, list_le, &list_count);
        break;
    }

    for (i = 0; i < opp->nb_cpus; i++) {
        opp->dst[i].irqs = g_new(qemu_irq, OPENPIC_OUTPUT_NB);
        for (j = 0; j < OPENPIC_OUTPUT_NB; j++) {
            sysbus_init_irq(d, &opp->dst[i].irqs[j]);
        }
    }

    register_savevm(dev, "openpic", 0, 2,
                    openpic_save, openpic_load, opp);

    sysbus_init_mmio(d, &opp->mem);
    qdev_init_gpio_in(dev, openpic_set_irq, opp->max_irq);
printf("helloworld");
}
