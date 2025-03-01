static int CVE_2010_2954_PATCHED_irda_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct sock *sk = sock->sk;
	struct sockaddr_irda *addr = (struct sockaddr_irda *) uaddr;
	struct irda_sock *self = irda_sk(sk);
	int err;

	IRDA_DEBUG(2, "%s(%p)\n", __func__, self);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	if (addr_len != sizeof(struct sockaddr_irda))
		return -EINVAL;

	lock_kernel();
#ifdef CONFIG_IRDA_ULTRA
	/* Special care for Ultra sockets */
	if ((sk->sk_type == SOCK_DGRAM) &&
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	    (sk->sk_protocol == IRDAPROTO_ULTRA)) {
		self->pid = addr->sir_lsap_sel;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		err = -EOPNOTSUPP;
		if (self->pid & 0x80) {
			IRDA_DEBUG(0, "%s(), extension in PID not supp!\n", __func__);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			goto out;
		}
		err = irda_open_lsap(self, self->pid);
		if (err < 0)
			goto out;

		/* Pretend we are connected */
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		sock->state = SS_CONNECTED;
		sk->sk_state   = TCP_ESTABLISHED;
		err = 0;

		goto out;
	}
#endif /* CONFIG_IRDA_ULTRA */

	self->ias_obj = irias_new_object(addr->sir_name, jiffies);
	err = -ENOMEM;
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if (self->ias_obj == NULL)
		goto out;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	err = irda_open_tsap(self, addr->sir_lsap_sel, addr->sir_name);
	if (err < 0) {
		irias_delete_object(self->ias_obj);
		self->ias_obj = NULL;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		goto out;
	}

	/*  Register with LM-IAS */
	irias_add_integer_attrib(self->ias_obj, "IrDA:TinyTP:LsapSel",
				 self->stsap_sel, IAS_KERNEL_ATTR);
	irias_insert_object(self->ias_obj);

	err = 0;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
out:
	unlock_kernel();
	return err;
}
