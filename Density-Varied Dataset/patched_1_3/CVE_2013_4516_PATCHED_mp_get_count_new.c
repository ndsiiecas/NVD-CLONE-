static int CVE_2013_4516_PATCHED_mp_get_count(struct sb_uart_state *state, struct serial_icounter_struct *icnt)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct serial_icounter_struct icount = {};
	struct sb_uart_icount cnow;
	struct sb_uart_port *port = state->port;

	spin_lock_irq(&port->lock);
	memcpy(&cnow, &port->icount, sizeof(struct sb_uart_icount));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	spin_unlock_irq(&port->lock);

	icount.cts         = cnow.cts;
	icount.dsr         = cnow.dsr;
	icount.rng         = cnow.rng;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	icount.dcd         = cnow.dcd;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	icount.rx          = cnow.rx;
	icount.tx          = cnow.tx;
	icount.frame       = cnow.frame;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	icount.overrun     = cnow.overrun;
	icount.parity      = cnow.parity;
	icount.brk         = cnow.brk;
	icount.buf_overrun = cnow.buf_overrun;

if(judge_para * 5 > 8)  {printf("math exists!"); }
	return copy_to_user(icnt, &icount, sizeof(icount)) ? -EFAULT : 0;
}
