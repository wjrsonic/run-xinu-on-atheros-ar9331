/* ttyInterrupt.c - ttyInterrupt */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyInterrupt - handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
interrupt ttyInterrupt(void)
{
	struct	dentry	*devptr;	/* pointer to devtab entry	*/
	struct	ttycblk	*typtr;		/* pointer to ttytab entry	*/	
	struct	uart_csreg *uptr;	/* address of UART's CSRs	*/
	int32	uart_cs = 0;
	int32	uart_int = 0;

	/* For now, the CONSOLE is the only serial device */

	devptr = (struct dentry *)&devtab[CONSOLE];

	/* Obtain the CSR address for the UART */

	uptr = (struct uart_csreg *)devptr->dvcsr;

	/* Obtain a pointer to the tty control block */

	typtr = &ttytab[ devptr->dvminor ];

	/* Decode hardware interrupt request from UART device */

	uart_int = uptr->uart_int;
	uart_int &= uptr->uart_int_en;

	if(uart_int & UART_RX_VALID_INT)
	{
		uptr->uart_int |= UART_RX_VALID_INT;
		ttyInter_in(typtr, uptr);
		return;
	}
	if(uart_int & UART_TX_EMPTY_INT)
	{
		uptr->uart_int |= UART_TX_EMPTY_INT;
		uptr->uart_int_en &= (~UART_TX_EMPTY_INT_EN);
		ttyInter_out(typtr, uptr);
		return;
	}

}
