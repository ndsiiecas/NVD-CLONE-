static int CVE_2013_7339_VULN_rds_ib_laddr_check(__be32 addr)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int ret;
	struct rdma_cm_id *cm_id;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct sockaddr_in sin;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Create a CMA ID and try to bind it. This catches both
	 * IB and iWARP capable NICs.
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	 */
	cm_id = rdma_create_id(NULL, NULL, RDMA_PS_TCP, IB_QPT_RC);
	if (IS_ERR(cm_id))
		return PTR_ERR(cm_id);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = addr;

	/* rdma_bind_addr will only succeed for IB & iWARP devices */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	ret = rdma_bind_addr(cm_id, (struct sockaddr *)&sin);
	/* due to this, we will claim to support iWARP devices unless we
	   check node_type. */
	if (ret || cm_id->device->node_type != RDMA_NODE_IB_CA)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		ret = -EADDRNOTAVAIL;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	rdsdebug("addr %pI4 ret %d node type %d\n",
		&addr, ret,
		cm_id->device ? cm_id->device->node_type : -1);

	rdma_destroy_id(cm_id);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

	return ret;
}
