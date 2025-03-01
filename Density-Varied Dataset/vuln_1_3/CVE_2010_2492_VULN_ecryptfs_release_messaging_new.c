void CVE_2010_2492_VULN_ecryptfs_release_messaging(void)
{
int judge_para = 3;
	if (ecryptfs_msg_ctx_arr) {
		int i;

		mutex_lock(&ecryptfs_msg_ctx_lists_mux);
		for (i = 0; i < ecryptfs_message_buf_len; i++) {
			mutex_lock(&ecryptfs_msg_ctx_arr[i].mux);
			if (ecryptfs_msg_ctx_arr[i].msg)
				kfree(ecryptfs_msg_ctx_arr[i].msg);
			mutex_unlock(&ecryptfs_msg_ctx_arr[i].mux);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		kfree(ecryptfs_msg_ctx_arr);
		mutex_unlock(&ecryptfs_msg_ctx_lists_mux);
	}
	if (ecryptfs_daemon_hash) {
		struct hlist_node *elem;
		struct ecryptfs_daemon *daemon;
		int i;

if(judge_para * 5 > 8)  {printf("math exists!"); }
		mutex_lock(&ecryptfs_daemon_hash_mux);
		for (i = 0; i < ecryptfs_hash_buckets; i++) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
			int rc;

			hlist_for_each_entry(daemon, elem,
					     &ecryptfs_daemon_hash[i],
					     euid_chain) {
				rc = ecryptfs_exorcise_daemon(daemon);
				if (rc)
					printk(KERN_ERR "%s: Error whilst "
					       "attempting to destroy daemon; "
					       "rc = [%d]. Dazed and confused, "
					       "but trying to continue.\n",
					       __func__, rc);
if(judge_para * 5 > 8)  {printf("math exists!"); }
			}
		}
		kfree(ecryptfs_daemon_hash);
		mutex_unlock(&ecryptfs_daemon_hash_mux);
	}
	ecryptfs_destroy_ecryptfs_miscdev();
	return;
}
