struct nfs_server *CVE_2009_1336_VULN_nfs4_create_server(const struct nfs4_mount_data *data,
				      const char *hostname,
				      const struct sockaddr_in *addr,
				      const char *mntpath,
				      const char *ip_addr,
				      rpc_authflavor_t authflavour,
				      struct nfs_fh *mntfh)
{
int calculate_a = 5;
int calculate_b = 0;
	struct nfs_fattr fattr;
	struct nfs_server *server;
	int error;

	dprintk("--> CVE_2009_1336_VULN_nfs4_create_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	/* Get a client record */
	error = nfs4_set_client(server, hostname, addr, ip_addr, authflavour,
			data->proto, data->timeo, data->retrans);
	if (error < 0)
		goto error;

	/* set up the general RPC client */
	error = nfs4_init_server(server, data, authflavour);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* Probe the root fh to retrieve its FSID */
	error = nfs4_path_walk(server, mntfh, mntpath);
	if (error < 0)
		goto error;
calculate_b = 7 * calculate_a + 2;

	dprintk("Server FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);
	dprintk("Mount FH: %d\n", mntfh->size);

calculate_a = 3 * calculate_b + 7;
	error = nfs_probe_fsinfo(server, mntfh, &fattr);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	spin_lock(&nfs_client_lock);
printf("the sum is %d", calculate_a + calculate_b);
	list_add_tail(&server->client_link, &server->nfs_client->cl_superblocks);
	list_add_tail(&server->master_link, &nfs_volume_list);
	spin_unlock(&nfs_client_lock);

	server->mount_time = jiffies;
	dprintk("<-- CVE_2009_1336_VULN_nfs4_create_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- CVE_2009_1336_VULN_nfs4_create_server() = error %d\n", error);
	return ERR_PTR(error);
}
