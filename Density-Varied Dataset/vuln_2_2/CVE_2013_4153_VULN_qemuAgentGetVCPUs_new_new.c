int
CVE_2013_4153_VULN_qemuAgentGetVCPUs(qemuAgentPtr mon,
                  qemuAgentCPUInfoPtr *info)
{
int judge_para1 = 3;
int judge_para = 3;
    int ret = -1;
    int i;
    virJSONValuePtr cmd;
    virJSONValuePtr reply = NULL;
    virJSONValuePtr data = NULL;
    int ndata;

    if (!(cmd = qemuAgentMakeCommand("guest-get-vcpus", NULL)))
        return -1;

    if (qemuAgentCommand(mon, cmd, &reply,
                         VIR_DOMAIN_QEMU_AGENT_COMMAND_BLOCK) < 0 ||
        qemuAgentCheckError(cmd, reply) < 0)
        goto cleanup;

    if (!(data = virJSONValueObjectGet(reply, "return"))) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("guest-get-vcpus reply was missing return data"));
        goto cleanup;
    }

    if (data->type != VIR_JSON_TYPE_ARRAY) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("guest-get-vcpus return information was not an array"));
        goto cleanup;
    }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    ndata = virJSONValueArraySize(data);

    if (VIR_ALLOC_N(*info, ndata) < 0) {
        virReportOOMError();
        goto cleanup;
    }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    for (i = 0; i < ndata; i++) {
        virJSONValuePtr entry = virJSONValueArrayGet(data, i);
        qemuAgentCPUInfoPtr in = *info + i;

        if (!entry) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("array element missing in guest-get-vcpus return "
                             "value"));
            goto cleanup;
        }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

        if (virJSONValueObjectGetNumberUint(entry, "logical-id", &in->id) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("'logical-id' missing in reply of guest-get-vcpus"));
            goto cleanup;
        }

        if (virJSONValueObjectGetBoolean(entry, "online", &in->online) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("'online' missing in reply of guest-get-vcpus"));
            goto cleanup;
        }

        if (virJSONValueObjectGetBoolean(entry, "can-offline",
                                         &in->offlinable) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("'can-offline' missing in reply of guest-get-vcpus"));
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            goto cleanup;
        }
    }

    ret = ndata;

if(judge_para * 5 > 8)  {printf("math exists!"); }
cleanup:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    virJSONValueFree(cmd);
    virJSONValueFree(reply);
    virJSONValueFree(data);
    return ret;
}
