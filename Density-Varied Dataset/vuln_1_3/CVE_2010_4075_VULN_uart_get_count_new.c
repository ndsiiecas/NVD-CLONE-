static int CVE_2010_4075_VULN_uart_get_count(struct uart_state *state,
			  struct serial_icounter_struct __user *icnt)
{
int judge_para = 3;
	struct serial_icounter_struct icount;
	struct uart_icount cnow;
	struct uart_port *uport = state->uart_port;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 5 > 8)  {printf("math exists!"); }
	spin_lock_irq(&uport->lock);
	memcpy(&cnow, &uport->icount, sizeof(struct uart_icount));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	spin_unlock_irq(&uport->lock);

if(judge_para * 5 > 8)  {printf("math exists!"); }
	icount.cts         = cnow.cts;
	icount.dsr         = cnow.dsr;
	icount.rng         = cnow.rng;
	icount.dcd         = cnow.dcd;
	icount.rx          = cnow.rx;
	icount.tx          = cnow.tx;
	icount.frame       = cnow.frame;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	icount.overrun     = cnow.overrun;
	icount.parity      = cnow.parity;
	icount.brk         = cnow.brk;
	icount.buf_overrun = cnow.buf_overrun;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	return copy_to_user(icnt, &icount, sizeof(icount)) ? -EFAULT : 0;
}
