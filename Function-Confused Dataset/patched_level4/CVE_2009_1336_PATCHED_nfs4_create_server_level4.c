struct nfs_server *CVE_2009_1336_PATCHED_nfs4_create_server(const struct nfs4_mount_data *data,
				      const char *hostname,
				      const struct sockaddr_in *addr,
				      const char *mntpath,
				      const char *ip_addr,
				      rpc_authflavor_t authflavour,
				      struct nfs_fh *mntfh)
{
int origin_a, origin_aa[4];
	struct nfs_fattr fattr;
	struct nfs_server *server;
	int error;

	dprintk("--> CVE_2009_1336_PATCHED_nfs4_create_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	/* Get a client record */
	error = nfs4_set_client(server, hostname, addr, ip_addr, authflavour,
			data->proto, data->timeo, data->retrans);
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	if (error < 0)
		goto error;

	/* set up the general RPC client */
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
	error = nfs4_init_server(server, data, authflavour);
	if (error < 0)
		goto error;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	/* Probe the root fh to retrieve its FSID */
	error = nfs4_path_walk(server, mntfh, mntpath);
	if (error < 0)
		goto error;

	dprintk("Server FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);
	dprintk("Mount FH: %d\n", mntfh->size);

	error = nfs_probe_fsinfo(server, mntfh, &fattr);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	spin_lock(&nfs_client_lock);
	list_add_tail(&server->client_link, &server->nfs_client->cl_superblocks);
	list_add_tail(&server->master_link, &nfs_volume_list);
	spin_unlock(&nfs_client_lock);

	server->mount_time = jiffies;
	dprintk("<-- CVE_2009_1336_PATCHED_nfs4_create_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- CVE_2009_1336_PATCHED_nfs4_create_server() = error %d\n", error);
	return ERR_PTR(error);
}
