static int
CVE_2014_3633_PATCHED_qemuDomainGetBlockIoTune(virDomainPtr dom,
                         const char *disk,
                         virTypedParameterPtr params,
                         int *nparams,
                         unsigned int flags)
{
int origin_a, origin_aa[4];
    struct qemud_driver *driver = dom->conn->privateData;
    virDomainObjPtr vm = NULL;
    qemuDomainObjPrivatePtr priv;
    virDomainDefPtr persistentDef = NULL;
    virDomainBlockIoTuneInfo reply;
    char uuidstr[VIR_UUID_STRING_BUFLEN];
    const char *device = NULL;
    int ret = -1;
    int i;

    virCheckFlags(VIR_DOMAIN_AFFECT_LIVE |
                  VIR_DOMAIN_AFFECT_CONFIG |
                  VIR_TYPED_PARAM_STRING_OKAY, -1);

    /* We don't return strings, and thus trivially support this flag.  */
    flags &= ~VIR_TYPED_PARAM_STRING_OKAY;

    qemuDriverLock(driver);
    virUUIDFormat(dom->uuid, uuidstr);
    vm = virDomainFindByUUID(&driver->domains, dom->uuid);
    if (!vm) {
        virReportError(VIR_ERR_NO_DOMAIN,
                       _("no domain with matching uuid '%s'"), uuidstr);
        goto cleanup;
    }

    if ((*nparams) == 0) {
        /* Current number of parameters supported by QEMU Block I/O Throttling */
        *nparams = QEMU_NB_BLOCK_IO_TUNE_PARAM;
        ret = 0;
        goto cleanup;
    }

    device = qemuDiskPathToAlias(vm, disk, NULL);

    if (!device) {
        goto cleanup;
    }

    if (qemuDomainObjBeginJobWithDriver(driver, vm, QEMU_JOB_MODIFY) < 0)
        goto cleanup;

    if (virDomainLiveConfigHelperMethod(driver->caps, vm, &flags,
                                        &persistentDef) < 0)
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        goto endjob;

    if (flags & VIR_DOMAIN_AFFECT_LIVE) {
        priv = vm->privateData;
        qemuDomainObjEnterMonitorWithDriver(driver, vm);
        ret = qemuMonitorGetBlockIoThrottle(priv->mon, device, &reply);
        qemuDomainObjExitMonitorWithDriver(driver, vm);
        if (ret < 0)
            goto endjob;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    }

    if (flags & VIR_DOMAIN_AFFECT_CONFIG) {
        int idx = virDomainDiskIndexByName(persistentDef, disk, true);
        if (idx < 0) {
            virReportError(VIR_ERR_INVALID_ARG,
                           _("disk '%s' was not found in the domain config"),
                           disk);
            goto endjob;
        }
        reply = persistentDef->disks[idx]->blkdeviotune;
    }

    for (i = 0; i < QEMU_NB_BLOCK_IO_TUNE_PARAM && i < *nparams; i++) {
        virTypedParameterPtr param = &params[i];

        switch(i) {
        case 0:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_TOTAL_BYTES_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.total_bytes_sec) < 0)
                goto endjob;
            break;
        case 1:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_READ_BYTES_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.read_bytes_sec) < 0)
                goto endjob;
            break;
        case 2:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_WRITE_BYTES_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.write_bytes_sec) < 0)
                goto endjob;
            break;
        case 3:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_TOTAL_IOPS_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.total_iops_sec) < 0)
                goto endjob;
            break;
        case 4:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_READ_IOPS_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.read_iops_sec) < 0)
                goto endjob;
            break;
        case 5:
            if (virTypedParameterAssign(param,
                                        VIR_DOMAIN_BLOCK_IOTUNE_WRITE_IOPS_SEC,
                                        VIR_TYPED_PARAM_ULLONG,
                                        reply.write_iops_sec) < 0)
                goto endjob;
            break;
        default:
            break;
        }
    }

    if (*nparams > QEMU_NB_BLOCK_IO_TUNE_PARAM)
        *nparams = QEMU_NB_BLOCK_IO_TUNE_PARAM;
    ret = 0;

endjob:
    if (qemuDomainObjEndJob(driver, vm) == 0)
        vm = NULL;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}

cleanup:
    VIR_FREE(device);
    if (vm)
        virDomainObjUnlock(vm);
    qemuDriverUnlock(driver);
    return ret;
}
