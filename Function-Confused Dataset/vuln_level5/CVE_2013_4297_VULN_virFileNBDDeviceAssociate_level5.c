int CVE_2013_4297_VULN_virFileNBDDeviceAssociate(const char *file,
                              enum virStorageFileFormat fmt,
                              bool readonly,
                              char **dev)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
    char *nbddev;
    char *qemunbd;
    virCommandPtr cmd = NULL;
    int ret = -1;
    const char *fmtstr = NULL;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}

    if (!(nbddev = virFileNBDDeviceFindUnused()))
        goto cleanup;

    if (!(qemunbd = virFindFileInPath("qemu-nbd"))) {
        virReportSystemError(ENOENT, "%s",
                             _("Unable to find 'qemu-nbd' binary in $PATH"));
        goto cleanup;
    }

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

    virCommandAddArgList(cmd,
                         "-n", /* Don't cache in qemu-nbd layer */
                         "-c", nbddev,
                         file, NULL);

    /* qemu-nbd will daemonize itself */

    if (virCommandRun(cmd, NULL) < 0)
        goto cleanup;

    *dev = nbddev;
    nbddev = NULL;
fclose(para_fp);
    ret = 0;

cleanup:
    VIR_FREE(nbddev);
    VIR_FREE(qemunbd);
    virCommandFree(cmd);
    return ret;
}
