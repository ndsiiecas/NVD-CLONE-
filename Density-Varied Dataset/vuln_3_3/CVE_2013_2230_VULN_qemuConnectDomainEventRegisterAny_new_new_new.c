static int
CVE_2013_2230_VULN_qemuConnectDomainEventRegisterAny(virConnectPtr conn,
                                  virDomainPtr dom,
                                  int eventID,
                                  virConnectDomainEventGenericCallback callback,
                                  void *opaque,
                                  virFreeCallback freecb)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    virQEMUDriverPtr driver = conn->privateData;
    int ret = -1;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

    if (virConnectDomainEventRegisterAnyEnsureACL(conn) < 0)
        goto cleanup;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    if (virDomainEventStateRegisterID(conn,
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                                      driver->domainEventState,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                                      dom, eventID,
                                      callback, opaque, freecb, &ret) < 0)
        goto cleanup;

    ret = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }

cleanup:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return ret;
}
