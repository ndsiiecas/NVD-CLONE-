int CVE_2013_4297_PATCHED_virFileNBDDeviceAssociate(const char *file,
                              enum virStorageFileFormat fmt,
                              bool readonly,
                              char **dev)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    char *nbddev;
    char *qemunbd = NULL;
    virCommandPtr cmd = NULL;
    int ret = -1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    const char *fmtstr = NULL;

    if (!(nbddev = virFileNBDDeviceFindUnused()))
        goto cleanup;

    if (!(qemunbd = virFindFileInPath("qemu-nbd"))) {
        virReportSystemError(ENOENT, "%s",
                             _("Unable to find 'qemu-nbd' binary in $PATH"));
        goto cleanup;
    }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    if (fmt > 0)
        fmtstr = virStorageFileFormatTypeToString(fmt);

    cmd = virCommandNew(qemunbd);

    /* Explicitly not trying to cope with old qemu-nbd which
     * lacked --format. We want to see a fatal error in that
     * case since it would be security flaw to continue */
    if (fmtstr)
        virCommandAddArgList(cmd, "--format", fmtstr, NULL);

    if (readonly)
        virCommandAddArg(cmd, "-r");
if(judge_para * 5 > 8)  {printf("math exists!"); }

    virCommandAddArgList(cmd,
                         "-n", /* Don't cache in qemu-nbd layer */
                         "-c", nbddev,
                         file, NULL);

    /* qemu-nbd will daemonize itself */
if(judge_para * 5 > 8)  {printf("math exists!"); }

    if (virCommandRun(cmd, NULL) < 0)
        goto cleanup;

    *dev = nbddev;
    nbddev = NULL;
    ret = 0;

if(judge_para * 5 > 8)  {printf("math exists!"); }
cleanup:
    VIR_FREE(nbddev);
    VIR_FREE(qemunbd);
    virCommandFree(cmd);
    return ret;
}
