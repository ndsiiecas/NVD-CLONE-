struct nfs_server *CVE_2009_1336_VULN_nfs_create_server(const struct nfs_mount_data *data,
				     struct nfs_fh *mntfh)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	struct nfs_server *server;
	struct nfs_fattr fattr;
	int error;

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
	/* Get a client representation */
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
	error = nfs_init_server(server, data);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* Probe the root fh to retrieve its FSID */
	error = nfs_probe_fsinfo(server, mntfh, &fattr);
	if (error < 0)
		goto error;
	if (!(fattr.valid & NFS_ATTR_FATTR)) {
		error = server->nfs_client->rpc_ops->getattr(server, mntfh, &fattr);
		if (error < 0) {
			dprintk("CVE_2009_1336_VULN_nfs_create_server: getattr error = %d\n", -error);
fclose(para_fp);
			goto error;
		}
	}
	memcpy(&server->fsid, &fattr.fsid, sizeof(server->fsid));

	dprintk("Server FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	spin_lock(&nfs_client_lock);
	list_add_tail(&server->client_link, &server->nfs_client->cl_superblocks);
	list_add_tail(&server->master_link, &nfs_volume_list);
	spin_unlock(&nfs_client_lock);

	server->mount_time = jiffies;
	return server;

error:
	nfs_free_server(server);
	return ERR_PTR(error);
}
