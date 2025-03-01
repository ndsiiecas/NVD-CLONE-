struct nfs_server *CVE_2009_1336_PATCHED_nfs4_create_referral_server(struct nfs_clone_mount *data,
					       struct nfs_fh *mntfh)
{
int judge_para1 = 3;
int judge_para = 3;
	struct nfs_client *parent_client;
	struct nfs_server *server, *parent_server;
	struct nfs_fattr fattr;
	int error;

	dprintk("--> CVE_2009_1336_PATCHED_nfs4_create_referral_server()\n");

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	parent_server = NFS_SB(data->sb);
	parent_client = parent_server->nfs_client;

	/* Get a client representation.
	 * Note: NFSv4 always uses TCP, */
	error = nfs4_set_client(server, data->hostname, data->addr,
			parent_client->cl_ipaddr,
			data->authflavor,
			parent_server->client->cl_xprt->prot,
			parent_client->retrans_timeo,
			parent_client->retrans_count);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (error < 0)
		goto error;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	/* Initialise the client representation from the parent server */
	nfs_server_copy_userdata(server, parent_server);
	server->caps |= NFS_CAP_ATOMIC_OPEN;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	error = nfs_init_server_rpcclient(server, data->authflavor);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* Probe the root fh to retrieve its FSID and filehandle */
	error = nfs4_path_walk(server, mntfh, data->mnt_path);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	if (error < 0)
		goto error;

	/* probe the filesystem info for this server filesystem */
	error = nfs_probe_fsinfo(server, mntfh, &fattr);
	if (error < 0)
		goto error;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

	dprintk("Referral FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

	spin_lock(&nfs_client_lock);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	list_add_tail(&server->client_link, &server->nfs_client->cl_superblocks);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	list_add_tail(&server->master_link, &nfs_volume_list);
	spin_unlock(&nfs_client_lock);

	server->mount_time = jiffies;

	dprintk("<-- nfs_create_referral_server() = %p\n", server);
	return server;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

error:
	nfs_free_server(server);
	dprintk("<-- CVE_2009_1336_PATCHED_nfs4_create_referral_server() = error %d\n", error);
	return ERR_PTR(error);
}
