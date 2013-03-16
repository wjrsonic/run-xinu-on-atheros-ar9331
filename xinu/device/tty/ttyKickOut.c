/* ttyKickOut.c - ttyKickOut */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyKickOut - "kick" the hardware for a tty device, causing it to
 *		generate an output interrupt (interrupts disabled)
 *------------------------------------------------------------------------
 */
void	ttyKickOut(
	 struct	ttycblk	*typtr,		/* ptr to ttytab entry		*/
	 struct uart_csreg *uptr	/* address of UART's CSRs	*/
	)
{
	/* Set output interrupts on the UART, which causes */
	/*   the device to generate an output interrupt    */

	uptr->uart_int |= UART_TX_EMPTY_INT;
	uptr->uart_int_en |= UART_TX_EMPTY_INT_EN;

	return;
}
