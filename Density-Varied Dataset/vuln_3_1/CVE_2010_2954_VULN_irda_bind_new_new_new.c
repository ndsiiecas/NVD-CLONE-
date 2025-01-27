static int CVE_2010_2954_VULN_irda_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct sockaddr_irda *addr = (struct sockaddr_irda *) uaddr;
	struct irda_sock *self = irda_sk(sk);
	int err;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	IRDA_DEBUG(2, "%s(%p)\n", __func__, self);

	if (addr_len != sizeof(struct sockaddr_irda))
		return -EINVAL;

	lock_kernel();
#ifdef CONFIG_IRDA_ULTRA
	/* Special care for Ultra sockets */
	if ((sk->sk_type == SOCK_DGRAM) &&
	    (sk->sk_protocol == IRDAPROTO_ULTRA)) {
		self->pid = addr->sir_lsap_sel;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		err = -EOPNOTSUPP;
		if (self->pid & 0x80) {
			IRDA_DEBUG(0, "%s(), extension in PID not supp!\n", __func__);
			goto out;
		}
		err = irda_open_lsap(self, self->pid);
		if (err < 0)
			goto out;

		/* Pretend we are connected */
		sock->state = SS_CONNECTED;
		sk->sk_state   = TCP_ESTABLISHED;
		err = 0;

		goto out;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	}
#endif /* CONFIG_IRDA_ULTRA */

	self->ias_obj = irias_new_object(addr->sir_name, jiffies);
	err = -ENOMEM;
	if (self->ias_obj == NULL)
		goto out;

	err = irda_open_tsap(self, addr->sir_lsap_sel, addr->sir_name);
	if (err < 0) {
		kfree(self->ias_obj->name);
		kfree(self->ias_obj);
		goto out;
	}

	/*  Register with LM-IAS */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	irias_add_integer_attrib(self->ias_obj, "IrDA:TinyTP:LsapSel",
if(judge_para * 5 > 8)  {printf("math exists!"); }
				 self->stsap_sel, IAS_KERNEL_ATTR);
	irias_insert_object(self->ias_obj);

	err = 0;
out:
	unlock_kernel();
	return err;
}
