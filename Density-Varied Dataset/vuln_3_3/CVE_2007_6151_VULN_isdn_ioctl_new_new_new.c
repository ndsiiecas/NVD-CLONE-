static int
CVE_2007_6151_VULN_isdn_ioctl(struct inode *inode, struct file *file, uint cmd, ulong arg)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	uint minor = iminor(inode);
	isdn_ctrl c;
	int drvidx;
	int chidx;
	int ret;
	int i;
	char __user *p;
	char *s;
	union iocpar {
		char name[10];
		char bname[22];
		isdn_ioctl_struct iocts;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		isdn_net_ioctl_phone phone;
		isdn_net_ioctl_cfg cfg;
	} iocpar;
	void __user *argp = (void __user *)arg;

#define name  iocpar.name
#define bname iocpar.bname
#define iocts iocpar.iocts
#define phone iocpar.phone
#define cfg   iocpar.cfg

	if (minor == ISDN_MINOR_STATUS) {
		switch (cmd) {
			case IIOCGETDVR:
				return (TTY_DV +
					(NET_DV << 8) +
					(INF_DV << 16));
			case IIOCGETCPS:
				if (arg) {
					ulong __user *p = argp;
					int i;
					if (!access_ok(VERIFY_WRITE, p,
							sizeof(ulong) * ISDN_MAX_CHANNELS * 2))
						return -EFAULT;
					for (i = 0; i < ISDN_MAX_CHANNELS; i++) {
						put_user(dev->ibytes[i], p++);
						put_user(dev->obytes[i], p++);
					}
					return 0;
				} else
					return -EINVAL;
				break;
#ifdef CONFIG_NETDEVICES
			case IIOCNETGPN:
				/* Get peer phone number of a connected 
				 * isdn network interface */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				if (arg) {
					if (copy_from_user(&phone, argp, sizeof(phone)))
						return -EFAULT;
					return isdn_net_getpeer(&phone, argp);
				} else
					return -EINVAL;
#endif
			default:
				return -EINVAL;
		}
	}
	if (!dev->drivers)
		return -ENODEV;
	if (minor <= ISDN_MINOR_BMAX) {
		drvidx = isdn_minor2drv(minor);
		if (drvidx < 0)
			return -ENODEV;
		chidx = isdn_minor2chan(minor);
		if (!(dev->drv[drvidx]->flags & DRV_FLAG_RUNNING))
			return -ENODEV;
		return 0;
	}
	if (minor <= ISDN_MINOR_CTRLMAX) {
/*
 * isdn net devices manage lots of configuration variables as linked lists.
 * Those lists must only be manipulated from user space. Some of the ioctl's
 * service routines access user space and are not atomic. Therefor, ioctl's
 * manipulating the lists and ioctl's sleeping while accessing the lists
 * are serialized by means of a semaphore.
 */
		switch (cmd) {
			case IIOCNETDWRSET:
				printk(KERN_INFO "INFO: ISDN_DW_ABC_EXTENSION not enabled\n");
				return(-EINVAL);
			case IIOCNETLCR:
				printk(KERN_INFO "INFO: ISDN_ABC_LCR_SUPPORT not enabled\n");
				return -ENODEV;
#ifdef CONFIG_NETDEVICES
			case IIOCNETAIF:
				/* Add a network-interface */
				if (arg) {
					if (copy_from_user(name, argp, sizeof(name)))
						return -EFAULT;
					s = name;
				} else {
					s = NULL;
				}
				ret = down_interruptible(&dev->sem);
				if( ret ) return ret;
				if ((s = isdn_net_new(s, NULL))) {
					if (copy_to_user(argp, s, strlen(s) + 1)){
						ret = -EFAULT;
					} else {
						ret = 0;
					}
				} else
					ret = -ENODEV;
				up(&dev->sem);
				return ret;
			case IIOCNETASL:
				/* Add a slave to a network-interface */
				if (arg) {
					if (copy_from_user(bname, argp, sizeof(bname) - 1))
						return -EFAULT;
				} else
					return -EINVAL;
				ret = down_interruptible(&dev->sem);
				if( ret ) return ret;
				if ((s = isdn_net_newslave(bname))) {
					if (copy_to_user(argp, s, strlen(s) + 1)){
						ret = -EFAULT;
					} else {
						ret = 0;
					}
				} else
					ret = -ENODEV;
				up(&dev->sem);
				return ret;
			case IIOCNETDIF:
				/* Delete a network-interface */
				if (arg) {
					if (copy_from_user(name, argp, sizeof(name)))
						return -EFAULT;
					ret = down_interruptible(&dev->sem);
					if( ret ) return ret;
					ret = isdn_net_rm(name);
					up(&dev->sem);
					return ret;
				} else
					return -EINVAL;
			case IIOCNETSCF:
				/* Set configurable parameters of a network-interface */
				if (arg) {
					if (copy_from_user(&cfg, argp, sizeof(cfg)))
						return -EFAULT;
					return isdn_net_setcfg(&cfg);
				} else
					return -EINVAL;
			case IIOCNETGCF:
				/* Get configurable parameters of a network-interface */
				if (arg) {
					if (copy_from_user(&cfg, argp, sizeof(cfg)))
						return -EFAULT;
					if (!(ret = isdn_net_getcfg(&cfg))) {
						if (copy_to_user(argp, &cfg, sizeof(cfg)))
							return -EFAULT;
					}
					return ret;
				} else
					return -EINVAL;
			case IIOCNETANM:
				/* Add a phone-number to a network-interface */
				if (arg) {
					if (copy_from_user(&phone, argp, sizeof(phone)))
						return -EFAULT;
					ret = down_interruptible(&dev->sem);
					if( ret ) return ret;
					ret = isdn_net_addphone(&phone);
					up(&dev->sem);
					return ret;
				} else
					return -EINVAL;
			case IIOCNETGNM:
				/* Get list of phone-numbers of a network-interface */
				if (arg) {
					if (copy_from_user(&phone, argp, sizeof(phone)))
						return -EFAULT;
					ret = down_interruptible(&dev->sem);
					if( ret ) return ret;
					ret = isdn_net_getphones(&phone, argp);
					up(&dev->sem);
					return ret;
				} else
					return -EINVAL;
			case IIOCNETDNM:
				/* Delete a phone-number of a network-interface */
				if (arg) {
					if (copy_from_user(&phone, argp, sizeof(phone)))
						return -EFAULT;
					ret = down_interruptible(&dev->sem);
					if( ret ) return ret;
					ret = isdn_net_delphone(&phone);
					up(&dev->sem);
					return ret;
				} else
					return -EINVAL;
			case IIOCNETDIL:
				/* Force dialing of a network-interface */
				if (arg) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
					if (copy_from_user(name, argp, sizeof(name)))
						return -EFAULT;
					return isdn_net_force_dial(name);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
				} else
					return -EINVAL;
#ifdef CONFIG_ISDN_PPP
			case IIOCNETALN:
				if (!arg)
					return -EINVAL;
				if (copy_from_user(name, argp, sizeof(name)))
					return -EFAULT;
				return isdn_ppp_dial_slave(name);
			case IIOCNETDLN:
				if (!arg)
					return -EINVAL;
				if (copy_from_user(name, argp, sizeof(name)))
					return -EFAULT;
				return isdn_ppp_hangup_slave(name);
#endif
			case IIOCNETHUP:
				/* Force hangup of a network-interface */
				if (!arg)
					return -EINVAL;
				if (copy_from_user(name, argp, sizeof(name)))
					return -EFAULT;
				return isdn_net_force_hangup(name);
				break;
#endif                          /* CONFIG_NETDEVICES */
			case IIOCSETVER:
				dev->net_verbose = arg;
				printk(KERN_INFO "isdn: Verbose-Level is %d\n", dev->net_verbose);
				return 0;
			case IIOCSETGST:
				if (arg)
					dev->global_flags |= ISDN_GLOBAL_STOPPED;
				else
					dev->global_flags &= ~ISDN_GLOBAL_STOPPED;
				printk(KERN_INFO "isdn: Global Mode %s\n",
				       (dev->global_flags & ISDN_GLOBAL_STOPPED) ? "stopped" : "running");
				return 0;
			case IIOCSETBRJ:
				drvidx = -1;
				if (arg) {
					int i;
					char *p;
					if (copy_from_user(&iocts, argp,
					     sizeof(isdn_ioctl_struct)))
						return -EFAULT;
					if (strlen(iocts.drvid)) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
						if ((p = strchr(iocts.drvid, ',')))
							*p = 0;
						drvidx = -1;
						for (i = 0; i < ISDN_MAX_DRIVERS; i++)
							if (!(strcmp(dev->drvid[i], iocts.drvid))) {
								drvidx = i;
								break;
							}
					}
				}
				if (drvidx == -1)
					return -ENODEV;
				if (iocts.arg)
					dev->drv[drvidx]->flags |= DRV_FLAG_REJBUS;
				else
					dev->drv[drvidx]->flags &= ~DRV_FLAG_REJBUS;
				return 0;
			case IIOCSIGPRF:
				dev->profd = current;
				return 0;
				break;
			case IIOCGETPRF:
				/* Get all Modem-Profiles */
				if (arg) {
					char __user *p = argp;
					int i;

					if (!access_ok(VERIFY_WRITE, argp,
					(ISDN_MODEM_NUMREG + ISDN_MSNLEN + ISDN_LMSNLEN)
						   * ISDN_MAX_CHANNELS))
						return -EFAULT;

					for (i = 0; i < ISDN_MAX_CHANNELS; i++) {
						if (copy_to_user(p, dev->mdm.info[i].emu.profile,
						      ISDN_MODEM_NUMREG))
							return -EFAULT;
						p += ISDN_MODEM_NUMREG;
						if (copy_to_user(p, dev->mdm.info[i].emu.pmsn, ISDN_MSNLEN))
							return -EFAULT;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
						p += ISDN_MSNLEN;
						if (copy_to_user(p, dev->mdm.info[i].emu.plmsn, ISDN_LMSNLEN))
							return -EFAULT;
						p += ISDN_LMSNLEN;
					}
					return (ISDN_MODEM_NUMREG + ISDN_MSNLEN + ISDN_LMSNLEN) * ISDN_MAX_CHANNELS;
				} else
					return -EINVAL;
				break;
			case IIOCSETPRF:
				/* Set all Modem-Profiles */
				if (arg) {
					char __user *p = argp;
					int i;

					if (!access_ok(VERIFY_READ, argp,
					(ISDN_MODEM_NUMREG + ISDN_MSNLEN + ISDN_LMSNLEN)
						   * ISDN_MAX_CHANNELS))
						return -EFAULT;

					for (i = 0; i < ISDN_MAX_CHANNELS; i++) {
						if (copy_from_user(dev->mdm.info[i].emu.profile, p,
						     ISDN_MODEM_NUMREG))
							return -EFAULT;
						p += ISDN_MODEM_NUMREG;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
						if (copy_from_user(dev->mdm.info[i].emu.plmsn, p, ISDN_LMSNLEN))
							return -EFAULT;
						p += ISDN_LMSNLEN;
						if (copy_from_user(dev->mdm.info[i].emu.pmsn, p, ISDN_MSNLEN))
							return -EFAULT;
						p += ISDN_MSNLEN;
					}
					return 0;
				} else
					return -EINVAL;
				break;
			case IIOCSETMAP:
			case IIOCGETMAP:
				/* Set/Get MSN->EAZ-Mapping for a driver */
				if (arg) {

					if (copy_from_user(&iocts, argp,
					     sizeof(isdn_ioctl_struct)))
						return -EFAULT;
					if (strlen(iocts.drvid)) {
						drvidx = -1;
						for (i = 0; i < ISDN_MAX_DRIVERS; i++)
							if (!(strcmp(dev->drvid[i], iocts.drvid))) {
								drvidx = i;
								break;
							}
					} else
						drvidx = 0;
					if (drvidx == -1)
						return -ENODEV;
					if (cmd == IIOCSETMAP) {
						int loop = 1;

						p = (char __user *) iocts.arg;
						i = 0;
						while (loop) {
							int j = 0;

							while (1) {
								if (!access_ok(VERIFY_READ, p, 1))
									return -EFAULT;
								get_user(bname[j], p++);
								switch (bname[j]) {
									case '\0':
										loop = 0;
										/* Fall through */
									case ',':
										bname[j] = '\0';
										strcpy(dev->drv[drvidx]->msn2eaz[i], bname);
										j = ISDN_MSNLEN;
										break;
									default:
										j++;
								}
								if (j >= ISDN_MSNLEN)
									break;
							}
							if (++i > 9)
								break;
						}
					} else {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
						p = (char __user *) iocts.arg;
						for (i = 0; i < 10; i++) {
							sprintf(bname, "%s%s",
								strlen(dev->drv[drvidx]->msn2eaz[i]) ?
								dev->drv[drvidx]->msn2eaz[i] : "_",
								(i < 9) ? "," : "\0");
							if (copy_to_user(p, bname, strlen(bname) + 1))
								return -EFAULT;
							p += strlen(bname);
						}
					}
					return 0;
				} else
					return -EINVAL;
			case IIOCDBGVAR:
				if (arg) {
					if (copy_to_user(argp, &dev, sizeof(ulong)))
						return -EFAULT;
					return 0;
				} else
					return -EINVAL;
				break;
			default:
				if ((cmd & IIOCDRVCTL) == IIOCDRVCTL)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
					cmd = ((cmd >> _IOC_NRSHIFT) & _IOC_NRMASK) & ISDN_DRVIOCTL_MASK;
				else
					return -EINVAL;
				if (arg) {
					int i;
					char *p;
					if (copy_from_user(&iocts, argp, sizeof(isdn_ioctl_struct)))
						return -EFAULT;
					if (strlen(iocts.drvid)) {
						if ((p = strchr(iocts.drvid, ',')))
							*p = 0;
						drvidx = -1;
						for (i = 0; i < ISDN_MAX_DRIVERS; i++)
							if (!(strcmp(dev->drvid[i], iocts.drvid))) {
								drvidx = i;
								break;
							}
					} else
						drvidx = 0;
					if (drvidx == -1)
						return -ENODEV;
					if (!access_ok(VERIFY_WRITE, argp,
					     sizeof(isdn_ioctl_struct)))
						return -EFAULT;
					c.driver = drvidx;
					c.command = ISDN_CMD_IOCTL;
					c.arg = cmd;
					memcpy(c.parm.num, &iocts.arg, sizeof(ulong));
					ret = isdn_command(&c);
					memcpy(&iocts.arg, c.parm.num, sizeof(ulong));
					if (copy_to_user(argp, &iocts, sizeof(isdn_ioctl_struct)))
						return -EFAULT;
					return ret;
				} else
					return -EINVAL;
		}
	}
#ifdef CONFIG_ISDN_PPP
	if (minor <= ISDN_MINOR_PPPMAX)
		return (isdn_ppp_ioctl(minor - ISDN_MINOR_PPP, file, cmd, arg));
if(judge_para * 5 > 8)  {printf("math exists!"); }
#endif
	return -ENODEV;

#undef name
#undef bname
#undef iocts
#undef phone
#undef cfg
}
