int CVE_2012_2669_PATCHED_main(void)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	int fd, len, sock_opt;
	int error;
	struct cn_msg *message;
	struct pollfd pfd;
	struct nlmsghdr *incoming_msg;
	struct cn_msg	*incoming_cn_msg;
	struct hv_kvp_msg *hv_msg;
	char	*p;
	char	*key_value;
	char	*key_name;

	daemon(1, 0);
	openlog("KVP", 0, LOG_USER);
	syslog(LOG_INFO, "KVP starting; pid is:%d", getpid());
	/*
	 * Retrieve OS release information.
	 */
	kvp_get_os_info();

	if (kvp_file_init()) {
		syslog(LOG_ERR, "Failed to initialize the pools");
		exit(-1);
	}

	fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (fd < 0) {
		syslog(LOG_ERR, "netlink socket creation failed; error:%d", fd);
		exit(-1);
	}
	addr.nl_family = AF_NETLINK;
	addr.nl_pad = 0;
	addr.nl_pid = 0;
	addr.nl_groups = CN_KVP_IDX;


	error = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (error < 0) {
		syslog(LOG_ERR, "bind failed; error:%d", error);
		close(fd);
		exit(-1);
	}
	sock_opt = addr.nl_groups;
	setsockopt(fd, 270, 1, &sock_opt, sizeof(sock_opt));
	/*
	 * Register ourselves with the kernel.
	 */
	message = (struct cn_msg *)kvp_send_buffer;
	message->id.idx = CN_KVP_IDX;
	message->id.val = CN_KVP_VAL;

	hv_msg = (struct hv_kvp_msg *)message->data;
	hv_msg->kvp_hdr.operation = KVP_OP_REGISTER;
	message->ack = 0;
	message->len = sizeof(struct hv_kvp_msg);

	len = netlink_send(fd, message);
	if (len < 0) {
		syslog(LOG_ERR, "netlink_send failed; error:%d", len);
		close(fd);
		exit(-1);
	}

	pfd.fd = fd;

	while (1) {
		struct sockaddr *addr_p = (struct sockaddr *) &addr;
		socklen_t addr_l = sizeof(addr);
		pfd.events = POLLIN;
		pfd.revents = 0;
		poll(&pfd, 1, -1);

		len = recvfrom(fd, kvp_recv_buffer, sizeof(kvp_recv_buffer), 0,
				addr_p, &addr_l);

		if (len < 0 || addr.nl_pid) {
			syslog(LOG_ERR, "recvfrom failed; pid:%u error:%d %s",
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
					addr.nl_pid, errno, strerror(errno));
			close(fd);
			return -1;
		}

		incoming_msg = (struct nlmsghdr *)kvp_recv_buffer;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		incoming_cn_msg = (struct cn_msg *)NLMSG_DATA(incoming_msg);
		hv_msg = (struct hv_kvp_msg *)incoming_cn_msg->data;

		switch (hv_msg->kvp_hdr.operation) {
		case KVP_OP_REGISTER:
			/*
			 * Driver is registering with us; stash away the version
			 * information.
			 */
			p = (char *)hv_msg->body.kvp_register.version;
			lic_version = malloc(strlen(p) + 1);
			if (lic_version) {
				strcpy(lic_version, p);
				syslog(LOG_INFO, "KVP LIC Version: %s",
					lic_version);
			} else {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
				syslog(LOG_ERR, "malloc failed");
			}
			continue;

		/*
		 * The current protocol with the kernel component uses a
		 * NULL key name to pass an error condition.
		 * For the SET, GET and DELETE operations,
		 * use the existing protocol to pass back error.
		 */

		case KVP_OP_SET:
			if (kvp_key_add_or_modify(hv_msg->kvp_hdr.pool,
					hv_msg->body.kvp_set.data.key,
					hv_msg->body.kvp_set.data.key_size,
					hv_msg->body.kvp_set.data.value,
					hv_msg->body.kvp_set.data.value_size))
				strcpy(hv_msg->body.kvp_set.data.key, "");
			break;

		case KVP_OP_GET:
			if (kvp_get_value(hv_msg->kvp_hdr.pool,
					hv_msg->body.kvp_set.data.key,
					hv_msg->body.kvp_set.data.key_size,
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
					hv_msg->body.kvp_set.data.value,
					hv_msg->body.kvp_set.data.value_size))
				strcpy(hv_msg->body.kvp_set.data.key, "");
			break;

		case KVP_OP_DELETE:
			if (kvp_key_delete(hv_msg->kvp_hdr.pool,
					hv_msg->body.kvp_delete.key,
					hv_msg->body.kvp_delete.key_size))
				strcpy(hv_msg->body.kvp_delete.key, "");
			break;

		default:
			break;
		}

		if (hv_msg->kvp_hdr.operation != KVP_OP_ENUMERATE)
			goto kvp_done;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		/*
		 * If the pool is KVP_POOL_AUTO, dynamically generate
		 * both the key and the value; if not read from the
		 * appropriate pool.
		 */
		if (hv_msg->kvp_hdr.pool != KVP_POOL_AUTO) {
			kvp_pool_enumerate(hv_msg->kvp_hdr.pool,
					hv_msg->body.kvp_enum_data.index,
					hv_msg->body.kvp_enum_data.data.key,
					HV_KVP_EXCHANGE_MAX_KEY_SIZE,
					hv_msg->body.kvp_enum_data.data.value,
					HV_KVP_EXCHANGE_MAX_VALUE_SIZE);
			goto kvp_done;
		}

		hv_msg = (struct hv_kvp_msg *)incoming_cn_msg->data;
		key_name = (char *)hv_msg->body.kvp_enum_data.data.key;
		key_value = (char *)hv_msg->body.kvp_enum_data.data.value;

		switch (hv_msg->body.kvp_enum_data.index) {
		case FullyQualifiedDomainName:
			kvp_get_domain_name(key_value,
					HV_KVP_EXCHANGE_MAX_VALUE_SIZE);
			strcpy(key_name, "FullyQualifiedDomainName");
			break;
		case IntegrationServicesVersion:
			strcpy(key_name, "IntegrationServicesVersion");
			strcpy(key_value, lic_version);
			break;
		case NetworkAddressIPv4:
			kvp_get_ip_address(AF_INET, key_value,
					HV_KVP_EXCHANGE_MAX_VALUE_SIZE);
			strcpy(key_name, "NetworkAddressIPv4");
			break;
		case NetworkAddressIPv6:
			kvp_get_ip_address(AF_INET6, key_value,
					HV_KVP_EXCHANGE_MAX_VALUE_SIZE);
			strcpy(key_name, "NetworkAddressIPv6");
			break;
		case OSBuildNumber:
			strcpy(key_value, os_build);
			strcpy(key_name, "OSBuildNumber");
			break;
		case OSName:
			strcpy(key_value, os_name);
			strcpy(key_name, "OSName");
			break;
		case OSMajorVersion:
			strcpy(key_value, os_major);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			strcpy(key_name, "OSMajorVersion");
			break;
		case OSMinorVersion:
			strcpy(key_value, os_minor);
			strcpy(key_name, "OSMinorVersion");
			break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		case OSVersion:
			strcpy(key_value, os_build);
			strcpy(key_name, "OSVersion");
			break;
		case ProcessorArchitecture:
			strcpy(key_value, processor_arch);
			strcpy(key_name, "ProcessorArchitecture");
			break;
		default:
			strcpy(key_value, "Unknown Key");
			/*
			 * We use a null key name to terminate enumeration.
			 */
			strcpy(key_name, "");
			break;
		}
		/*
		 * Send the value back to the kernel. The response is
		 * already in the receive buffer. Update the cn_msg header to
		 * reflect the key value that has been added to the message
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		 */
kvp_done:

		incoming_cn_msg->id.idx = CN_KVP_IDX;
		incoming_cn_msg->id.val = CN_KVP_VAL;
		incoming_cn_msg->ack = 0;
		incoming_cn_msg->len = sizeof(struct hv_kvp_msg);

		len = netlink_send(fd, incoming_cn_msg);
		if (len < 0) {
			syslog(LOG_ERR, "net_link send failed; error:%d", len);
			exit(-1);
		}
	}

}
